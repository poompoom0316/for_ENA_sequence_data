#!/bin/bash
#list_file_path="Data/filereport_read_run_PRJDB7217_tsv.txt"

for list_file_path in "Data/filereport_read_run_PRJDB7213_tsv.txt" "Data/filereport_read_run_PRJDB7217_tsv.txt"
do
  awk -F '\t'  '{print $2, $7}' "${list_file_path}" |grep SAMD|while read line
  do
    list=(${line//;/ })
    echo ${list[0]}
    echo ${list[1]}
    echo ${list[2]}
  done
done

