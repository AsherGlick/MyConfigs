# sync saves anything in the cache to the disk
# Writing 3 into the file /proc/sys/vm/drop_caches commands the system to empty all the caches
# This will free up your ram but can slow down your computer while the cashes are rebuilt (might be what you want)
sync && echo 3 > /proc/sys/vm/drop_caches
