/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file. Write operation will be denied. 
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <crypto/hash.h>

/*
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
char *get_hash(char *hash_type, char *original_sentence);
static struct sdesc *init_sdesc(struct crypto_shash *alg);
static int calc_hash(struct crypto_shash *alg, 
        const unsigned char *data, unsigned int datalen, unsigned char *digest);
int compare_hashes(const char* received_hash_str, 
        const unsigned char* computed_hash, size_t hash_size);

#define SUCCESS 0
#define DEVICE_NAME "CSC1107_11_kernel"   /* Dev name as it appears in /proc/devices   */
#define BUF_LEN 100              /* Max length of the message from the device */
#define HASH_MAX 64

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;               /* Major number assigned to our device driver */
static int Device_Open = 0;     /* Is device open?
                                 * Used to prevent multiple access to device */
static char msg[BUF_LEN];       /* The msg the device will give when asked */
static char *msg_Ptr;

static char *lines[3];
static int l_count = 0;
static char *temp;
static char *line;
static const char *delimiter = "\n";

static struct class *cls;

struct sdesc {
    struct shash_desc shash;
    char ctx[];
};

static struct file_operations kernel_fops =
{
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &kernel_fops);

    if (Major < 0) 
    {
        // pr_alert: printk with KERN_ALERT an alert-level message; 
        pr_alert("Registering char device failed with %d\n", Major);
        return Major;
    }

    //pr_info: printk with KERN_INFO loglevel, an info-level message
    // pr_info("I was assigned major number %d.\n", Major);
    printk(KERN_INFO "I was assigned major number %d.\n", Major);

    cls = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cls, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);

    //pr_info: printk with KERN_INFO loglevel, an info-level message
    pr_info("Device created on /dev/%s\n", DEVICE_NAME);

    return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
    device_destroy(cls, MKDEV(Major, 0));
    class_destroy(cls);

    /*
     * Unregister the device
     */
    unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
    if (Device_Open)
        return -EBUSY;

    Device_Open++;

    msg_Ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;          /* We're now ready for our next caller */

    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     */
    module_put(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
                           char *buffer,        /* buffer to fill with data */
                           size_t length,       /* length of the buffer     */
                           loff_t * offset)
{
    /*
     * Number of bytes actually written to the buffer
     */
    int bytes_read = 0;

    /*
     * If we're at the end of the message,
     * return 0 signifying end of file
     */
    if (*msg_Ptr == 0)
        return 0;

    /*
     * Actually put the data into the buffer
     */
    while (length && *msg_Ptr) {

        /*
         * The buffer is in the user data segment, not the kernel
         * segment so "*" assignment won't work.  We have to use
         * put_user which copies data from the kernel data segment to
         * the user data segment.
         */
        put_user(*(msg_Ptr++), buffer++);

        length--;
        bytes_read++;
    }

    /*
     * Most read functions return the number of bytes put into the buffer
     */
    return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t device_write(struct file *filp,
                            const char *buff,
                            size_t len,
                            loff_t * off)
{
    char *hash;
    int digest_size;

    char *hash_received;

    if (len > sizeof(msg) - 1)
        len = sizeof(msg) - 1;

    strncpy(msg, buff, len);
    msg[len] = '\0';   // Ensure the message is null-terminated

    // kernel message
    //printk(KERN_INFO "Data written to device:\n %s\n", msg);


    temp = msg;
    while ((line = strsep(&temp, delimiter)) != NULL)
    {
        if (l_count >= 3)
        {
            break;
        }
        lines[l_count++] = line;
    }

    /* Printing the original sentence received */
    printk(KERN_INFO "Original sentence received: %s\n", lines[0]);

    /* Printing the type of hash received */
    // TODO: Hash is not being received in full
    printk(KERN_INFO "Type of hash received: %s\n", lines[1]);

    /* Printing the hash received until the first \n */
    hash_received = lines[2];
    printk(KERN_INFO "Hashed sentence received: %s\n", hash_received);

    /* Calculating the hash in kernel */
    hash = get_hash(lines[1], lines[0]);

    /* Getting the size of the hash calculated in kernel for comparison */
    digest_size = strlen(hash);

    printk(KERN_INFO "Hash calculated in Kernel: ");

    /* Printing the hash calculated in kernel by converting it to a hex string */
    for (int i = 0; i < digest_size; i++)
    {
        printk(KERN_CONT "%02x", hash[i]);
    }
    printk(KERN_CONT "\n");

    /* 
     * Comparing the hash received and the hash calculated in kernel 
     * This is done by comparing the size of the hashes and 
     * then comparing the hashes themselves
     */
    if (strlen(hash_received) == 2 * digest_size && 
            compare_hashes(hash_received, hash, digest_size) == 0)
    {
        printk(KERN_INFO "Hashes match!\n");
    }
    else
    {
        printk(KERN_INFO "Hashes don't match!\n");
    }

    msg_Ptr = msg;
    return len;
}

/*
 * This function is called to initialize the sdesc struct, which is used to
 * calculate the hash of the message
 */
static struct sdesc *init_sdesc(struct crypto_shash *alg)
{
    /* sdesc is a struct that contains the shash struct */
    struct sdesc *sdesc;                                        
    int size;                                                 

    /* Calculate the size of the sdesc struct */
    size = sizeof(struct shash_desc) + crypto_shash_descsize(alg);

    /*  Allocate memory for the sdesc struct */
    sdesc = kmalloc(size, GFP_KERNEL);
    if (!sdesc)
        return ERR_PTR(-ENOMEM);

    /* Initialize the shash struct */
    sdesc->shash.tfm = alg;
    return sdesc;
}

/*
 * This function is called to calculate the hash of the message
 */
static int calc_hash(struct crypto_shash *alg,
                    const unsigned char *data, 
                    unsigned int datalen,
                    unsigned char *digest)
{
    /* sdesc is a struct that contains the shash struct */
    struct sdesc *sdesc;
    int ret;

    /* Initialize the sdesc struct and check for errors */
    sdesc = init_sdesc(alg);
    if (IS_ERR(sdesc))
        return PTR_ERR(sdesc);

    /* Calculate the hash and check for errors with the crypto_shash_digest function */
    ret = crypto_shash_digest(&sdesc->shash, data, datalen, digest); 

    /* Free the memory allocated for the sdesc struct */
    kfree(sdesc);
    return ret;
}


/*
 * This function is called to compute the hash of the original sentence
 * and compare it with the hash received from the user space
 */
char *get_hash(char *hash_type, char *original_sentence)
{
    struct crypto_shash *alg;
    char *hash_alg_name;
    unsigned int data_len = strlen(original_sentence);
    unsigned char *hashed_sentence;
    unsigned int digest_size;

    //printk(KERN_INFO "Hash type: %s\n", hash_type);

    /* Strip the \n from the hash type */
    hash_type = strsep(&hash_type, "\n");
    
    /* Check the hash type */
    if (strcmp(hash_type, "MD5") == 0)
    {
        hash_alg_name = "md5";
    }
    else if (strcmp(hash_type, "SHA1") == 0)
    {
        hash_alg_name = "sha1";
    }
    else if (strcmp(hash_type, "SHA256") == 0)
    {
        hash_alg_name = "sha256";
    }
    else if (strcmp(hash_type, "SHA384") == 0)
    {
        hash_alg_name = "sha384";
    }
    else if (strcmp(hash_type, "SHA512") == 0)
    {
        hash_alg_name = "sha512";
    }
    else
    {
        printk(KERN_ERR "Invalid hash type\n");
        return NULL;
    }

    /* Allocate memory for the hash algorithm */
    alg = crypto_alloc_shash(hash_alg_name, 0, 0);

    if (IS_ERR(alg)) {
        printk(KERN_ERR "Can't allocate hash algorithm %s\n", hash_type);
        return NULL;
    }

    /* Get the size of the used hash algorithm */
    digest_size = crypto_shash_digestsize(alg);

    /* Allocate memory for the hash */
    hashed_sentence = kmalloc(crypto_shash_digestsize(alg), GFP_KERNEL);
    if (!hashed_sentence) {
        printk(KERN_ERR "Unable to allocate digest\n");
        return NULL;
    }

    /* Calculate the hash of the original sentence */
    calc_hash(alg, original_sentence, data_len, hashed_sentence);

    /* Free the memory allocated for the hash algorithm */
    crypto_free_shash(alg);

    hashed_sentence[digest_size] = '\0';

    /* Free the memory allocated for the hash */
    kfree(hashed_sentence);                     
    return hashed_sentence;
}

/*
 * This function is called to compare the hash received from the user space
 * with the hash computed in the kernel
 */
int compare_hashes(const char* received_hash_str, 
                const unsigned char* computed_hash, 
                size_t hash_size)
{
    int result = -1;
    /* Allocate memory for the received hash and check if the allocation was successful */
    unsigned char* received_hash = kmalloc(hash_size, GFP_KERNEL);
    if (received_hash == NULL)
    {
        return -1;
    }

    /* Convert the received hash from hex to binary */
    for (size_t i = 0; i < hash_size; i++)
    {
        sscanf(&received_hash_str[2 * i], "%2hhx", &received_hash[i]);
    }

    /* Compare the received hash with the computed hash */
    result = memcmp(received_hash, computed_hash, hash_size);

    /* Don't forget to free the memory */
    kfree(received_hash);

    return result;
}
/*char* get_hash(char *hash_alg, char *input_string) {
    struct crypto_shash* algorithm;
    struct shash_desc* desc;
    int err;
    char output[1024];

	algorithm = crypto_alloc_shash(hash_alg, 0, 0); break;

	// Check if selected algorithm is available in the system
	if(IS_ERR(algorithm)) { // Check errors
		printk(KERN_ALERT "Hashing algorithm not supported\n");
		return -EFAULT;
	}

	desc = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(algorithm), GFP_KERNEL);
	if(!desc) { // check errors
		printk(KERN_ALERT "Failed to allocate memory(kmalloc)\n");
		return -ENOMEM;
	}
	desc->tfm = algorithm;

	// Initialize shash API
	err = crypto_shash_init(desc);
	if(err)  {
		printk(KERN_ALERT  "Failed to initialize shash\n");
		goto out;
	}

	// Execute hash function
	err = crypto_shash_update(desc, input_string, strlen(input_string));
	if(err) {
		printk(KERN_ALERT  "Failed to execute hashing function\n");
		goto out;
	}

	// Write the result to a new char buffer
	err = crypto_shash_final(desc, output);
	if(err) {
		pr_err("%s: Failed to complete hashing function\n", DEV_NAME);
		goto out;
	}

	// Finally, clean up resources
	crypto_free_shash(algorithm);
	kfree(desc);

	pr_info("%s: String successfully hashed. Read from this device to get the result\n", DEV_NAME);

	return output;

out: // Manage errors
	crypto_free_shash(algorithm);
	kfree(desc);
	return NULL;
}*/

MODULE_LICENSE("GPL");                  	// The license type 
MODULE_DESCRIPTION("Hash compare"); // The description of the module
MODULE_AUTHOR("Group 11");                     // The author of the module
MODULE_VERSION("0.1a");                  	// The version of the module

