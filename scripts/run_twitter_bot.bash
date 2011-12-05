#!/bin/bash

home="/home/balaji/balajinix/code/twitter_bot"

cd ${home}/scripts

bin_dir=${home}/bin
data_dir=${home}/data
log_dir=${home}/log

echo "start `date`"

echo "${bin_dir}/t_target_finder 0 \"balaji%20OR%20balajnix&geocode=12.984722,80.251944,40mi\" balajinix ${data_dir}/handles_master.txt ${data_dir}/handles_to_spam.txt ${data_dir}/tweet_id_class_name_map.txt"
${bin_dir}/t_target_finder 0 "balaji%20OR%20balajnix&geocode=12.984722,80.251944,40mi" balajinix ${data_dir}/handles_master.txt ${data_dir}/handles_to_spam.txt ${data_dir}/tweet_id_class_name_map.txt

echo "${bin_dir}/t_spammer <user_name> <password> ${data_dir}/handles_master.txt ${data_dir}/handles_to_spam.txt ${data_dir}/tweet_id_class_name_map.txt ${data_dir}/messages_file.txt"
${bin_dir}/t_spammer <user_name> <password> ${data_dir}/handles_master.txt ${data_dir}/handles_to_spam.txt ${data_dir}/tweet_id_class_name_map.txt ${data_dir}/messages_file.txt

echo "end `date`"
