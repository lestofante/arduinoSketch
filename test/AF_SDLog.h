
#ifndef _AF_SDLOG_h_
#define _AF_SDLOG_h_

#include "sd_raw.h"
#include "sd_raw_config.h"
#include "sd_reader_config.h"
#include "partition.h"
#include "partition_config.h"
#include "fat16.h"

typedef struct fat16_file_struct * File;

class AF_SDLog {
  struct partition_struct *partition;
  struct fat16_fs_struct* fs;
  struct fat16_dir_struct* dd;
  struct fat16_dir_entry_struct file_entry;

 public:
  AF_SDLog(void);
  uint8_t init_card(void);
  uint8_t open_partition(void);
  uint8_t open_filesys(void);
  uint8_t open_dir(char *path);
  uint8_t close_dir(void);
  char *get_next_name_in_dir(void);
  void reset_dir(void);
  File open_file(char *name);
  void close_file(File f);
  uint8_t create_file(char *name);
  uint8_t write_file(File f, uint8_t *b, uint8_t num);
  uint8_t seek_file(File fd, int32_t *offset, uint8_t whence);
};

#endif
