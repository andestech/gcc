#! /usr/bin/env python2
import re
import sys

def usage():
    print "%s <filter-file1>[,<filter-file2>[,...]] <sum file> <log file> <output> [--compiler=[clang|gcc]]" % sys.argv[0]

if len(sys.argv) < 5:
    usage()
    sys.exit(1)

is_clang = True
if len(sys.argv) >= 6:
    compilers = ['clang', 'gcc']
    compiler = 'clang'
    if sys.argv[5].startswith('--compiler='):
        compiler = sys.argv[5][len('--compiler='):]
    if compiler not in compilers:
        raise Exception("Unknown compiler `%s`" % compiler)
    is_clang = compiler == 'clang'

out_file = None
out_file_path = None

def output(s):
    out_file.write(s + "\n")
    print s

def read_ignore_list(ignore_list_paths):
    all_ignore_list = []
    for f in ignore_list_paths:
        with open(f, "r") as ignore_list_f:
            ignore_list = ignore_list_f.readlines()

        ignore_list = map(lambda l: l.strip(), ignore_list)
        # Filter out comment line and empty line
        ignore_list = filter(lambda l: not l.startswith("#") \
                                       and len(l) != 0, ignore_list)
        all_ignore_list += ignore_list
    return all_ignore_list

def count_case(prefix, data):
   f = filter(lambda s:s.startswith(prefix), data)
   total_count = len(f)
   x = set(map(lambda d: d.split(" ")[1] , f))
   case_count = len(x)
   return total_count, case_count

def find_begin_line(idx, log_data):
  idx = idx - 1
  while idx != 0:
    s = log_data[idx]
    if len(s) > 0 and s[0].isupper():
      return idx
    idx = idx - 1
  raise Exception("find_begin_line fail")

def read(sum_path, log_path, filter_files, is_clang=True):
  with open(sum_path, "r") as sum_file:
    sum_data = sum_file.readlines()

  with open(log_path, "r") as log_file:
    log_data = log_file.readlines()

  def filter_func(s):
    return s.startswith("FAIL:") \
           or s.startswith("XPASS:") \
           or s.startswith("UNRESOLVED:")

  # First, filter out only FAIL, XPASS and UNRESOLVED
  sum_data = filter(filter_func, sum_data)
  # Only clang need to filter out warnings and scan dumps.
  if is_clang:
      # And then filter out all "test for warnings"
      sum_data = filter(lambda s:"(test for warnings" not in s \
                                 and "FAIL: compiler driver" not in s \
                                 and "(test for errors, line" not in s \
                                 and "(test for excess errors" not in s \
                                 and "(test for bogus messages, line" not in s,sum_data)
      # Filter out all "scan-tree-dump", "scan-rtl-dump" and "scan-ipa-dump-times"
      sum_data = filter(lambda s:" scan-tree-dump" not in s \
                                 and " scan-rtl-dump" not in s \
                                 and " expected multiline pattern" not in s \
                                 and " scan-ipa-dump" not in s,sum_data)

  ignore_list = read_ignore_list(filter_files)
  def filter_ignore(s):
    for ignore in ignore_list:
      if ignore in s:
        return False
    return True

  # Last, filter out all contain in ignore_list.
  sum_data = filter(filter_ignore, sum_data)

  # Disable filtered log gen.
  if is_clang and False:
      # Create a filtered log
      find_count = 0
      not_found_count = 0
      start_index = 0
      log_data_len = len(log_data)
      with open(log_path + ".filtered", "w") as f:
          for result in sum_data:
              # Find the result in where
              idx = start_index
              while idx < log_data_len:
                  log = log_data[idx]
                  if log == result:
                      find_count = find_count + 1
                      start_index = idx
                      begin_line = find_begin_line(idx, log_data)
                      f.write("".join(log_data[begin_line+1:idx+1]))
                      break
                  idx = idx + 1
              else:
                  raise Exception("Not found match entry `%s` in log(%s not match %s?)" \
                                  % (result, log_path, sum_path))

  fail_count, fail_case_count = count_case("FAIL:", sum_data)
  xpass_count, xpass_case_count = count_case("XPASS:", sum_data)
  unresolve_count, unresolve_case_count = count_case("UNRESOLVED:", sum_data)
  with open(sum_path + ".filtered", "w") as f:
    f.write("".join(sum_data))
  if len(sum_data) > 100:
    out_file.write("".join(sum_data))
    print "".join(sum_data[0:100])
    print "... (detail info in %s)" % out_file_path
  else:
    output( "".join(sum_data))
  output( "%-28s %6d (%6d case)" % ("# of unexpected failures", fail_count, fail_case_count))
  output( "%-28s %6d (%6d case)" % ("# of unexpected successes", xpass_count, xpass_case_count))
  output( "%-28s %6d (%6d case)" % ("# of unresolved testcases", unresolve_count, unresolve_case_count))
  fail = fail_count + xpass_count + unresolve_count

  if fail != 0:
    return 1
  else:
    return 0

filter_files = sys.argv[1].split(',')
sum_file = sys.argv[2]
log_file = sys.argv[3]
out_file_path = sys.argv[4]
with file(out_file_path, "w") as out_file:
  rv = read(sum_file, log_file, filter_files, is_clang)
sys.exit(rv)
