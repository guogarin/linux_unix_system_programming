#!/usr/bin/python3
#coding: utf-8
# create by guohang
# 此脚本用于分析产环境日志


import sys
import re
import csv


re_bank_id = re.compile(r"\[\d{4}\]")
bank_dict = {
    '0102'   : "工商银行" ,
    '0104'   : "中国银行" ,
    '0105'   : "建设银行" ,
    '0106'   : "交通银行" ,
    '0107'   : "招商银行" ,
    '0108'   : "浦发银行" ,
    '0103'   : "农业银行" ,
    '0201'   : "港中银  " ,
    '0111'   : "上海银行" ,
    '0109'   : "民生银行" ,
    }

Errno_dict = {
    "[Errno 110]": " 连接超时"

    }


base_path = "/home/intl2/log/"

erro_dict = {}  # erro_name:{bank_id : num}

def main(trade_date):
    log_file_path = "%s%s/intl_bank_acsvr/10/bankacsvr.log"%(base_path,trade_date)
    with open(log_file_path, "r") as f_log:
        for line in f_log:
            if line.find("WARNING") > 0:
                bank_id = re.findall(re_bank_id, line)
                if len(bank_id) != 0:
                    bank_id = bank_id[0].replace("[", "")
                    bank_id = bank_id.replace("]", "")
                else:
                    print("WARNING日志，但未找到bankID")
                    print(bank_id)
                    print(line)
                    continue
                #print(bank_id)
                if bank_id not in erro_dict:
                    erro_dict[bank_id] = {}

                if line.find("Errno 110") > 0:
                    if "连接超时" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["连接超时"] =  1
                    else:
                        erro_dict[bank_id]["连接超时"] += 1
                elif line.find("Errno 104") > 0:
                    if "银行端已关闭连接(Connection reset by peer)" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["银行端已关闭连接(Connection reset by peer)"] =  1
                    else:
                        erro_dict[bank_id]["银行端已关闭连接(Connection reset by peer)"] +=  1
                elif line.find("Errno 107") > 0:
                    if "Transport endpoint is not connected" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["Transport endpoint is not connected"]  =  1
                    else:
                        erro_dict[bank_id]["Transport endpoint is not connected"] +=  1
                elif line.find("Errno 32") > 0:
                    if "Broken pipe" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["Broken pipe"]  =  1
                    else:
                        erro_dict[bank_id]["Broken pipe"]  += 1
                elif line.find("Errno 111") > 0:
                    if "Connection refused" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["Connection refused"] =  1
                    else:
                        erro_dict[bank_id]["Connection refused"] +=  1

                elif line.find("recv gcp message error") > 0:
                    if "接收gcp消息失败" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["接收gcp消息失败"] =  1
                    else:
                        erro_dict[bank_id]["接收gcp消息失败"] += 1
                elif line.find("heartbeat timeout") > 0:
                    if "心跳超时" not in erro_dict[bank_id]:
                        erro_dict[bank_id]["心跳超时"] =  1
                    else:
                        erro_dict[bank_id]["心跳超时"] += 1
                elif line.find("send gcp data error") > 0:
                    if "发送gcp消息失败"not in erro_dict[bank_id]:
                        erro_dict[bank_id]["发送gcp消息失败"] =  1
                    else:
                        erro_dict[bank_id]["发送gcp消息失败"] += 1
                elif line.find("<_io.TextIOWrapper name='<stderr>' mode='w' encoding='UTF-8'>") > 0:
                    continue
                else:
                    print(line)
            else:
                continue
    
    csv_file = "/home/intl2/guohang/bankacsvr_parser/parse_result/banacsvr%s.csv"%trade_date
    with open(csv_file, "w", newline="") as f_csv:
        csv_writer = csv.writer(f_csv)
        csv_writer.writerow(("银行", "错误名", "次数"))
        for bank_id in erro_dict:
            for erro in erro_dict[bank_id]:
                csv_writer.writerow(("%s(%s)"%(bank_dict[bank_id], bank_id) ,erro ,erro_dict[bank_id][erro]))
                print(("%s(%s)"%(bank_dict[bank_id], bank_id) ,erro ,erro_dict[bank_id][erro]))


if __name__ == "__main__":
    if len(sys.argv) ==  2:
        main(sys.argv[1])
    else:
        print("Usage: python3 bankacsvr_parser.py trade_date")
