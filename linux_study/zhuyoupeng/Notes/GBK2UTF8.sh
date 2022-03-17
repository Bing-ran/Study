#!/bin/bash
#@liangzhiwen 2021.06.10
#For convert GBK encoding files to UTF-8 encoding.
echo "========================================="
echo "======== GBK to UTF-8 Tools V1.0 ========"
echo "========================================="

isGeditRunning=`ps -ef | grep gedit | grep -v grep`
if [ ! -z "$isGeditRunning" ]
then
	echo "Please close gedit before run this bash shell."
	exit 1
fi

if [ 1 -gt $# ]
then
	echo "usage:./xxx.sh file_list.txt"
	exit 1
fi

arg1="$1"
case $arg1 in
	"-h")
		;&
	"--help")
		echo "Uage: ./xxx.sh [OPTION...][FILENAME]"
		echo "  -h,--help		Type help information."
		echo "  -v,--version		Type version information."
		echo "  --list-h		Execute:file find ./ -name *.h | grep ISO- > h.txt."
		echo "  --list-c		Execute:file find ./ -name *.c | grep ISO- > c.txt."
		echo "  filename		Which contain convert filename list per line, create by --list-h or --list-c."
		exit 0
		;;
	"-v")
		;&
	"--version")
		echo "GBK to UTF8 Tools V1.0 @2021.06.10"
		exit 0
		;;
	"--list-h")
		echo '    --> find `find ./ -name *.h` | grep ISO- > h.txt'
		file `find ./ -name *.h `| grep ISO- > h.txt
		exit 0
		;;
	"--list-c")
		echo '    --> find `find ./ -name *.c` | grep ISO- > c.txt'
		file `find ./ -name *.c `| grep ISO- > c.txt
		exit 0
		;;
esac

if [ ! -f "$arg1" ]
then
	echo "$arg1 not found"
	exit 1
fi

#GBK编码转UTF-8编码
#0:convert failed, 1:success
function GBK2UTF8()
{
	local convert_result=0
	ret="`file "$1"`"
	isUTF8="`echo $ret | grep UTF-8`"
	if [ -z "$isUTF8" ]
	then
		#echo "    ---> iconv -f GBK -t UTF-8 "$1" -o "$2" && mv "$2" "$1" && gedit "$1""
		#`iconv -f GBK -t UTF-8 "$1" -o "$1" && gedit "$1"`
		#`iconv -f GBK -t UTF-8 "$1" -o "$2" && meld "$1" "$2"`
		`iconv -f GBK -t UTF-8 "$1" -o "$2" && mv "$2" "$1" && gedit "$1"`
		ret="`file "$1"`"
		isUTF8="`echo $ret | grep UTF-8`"
		if [ -z "$isUTF8" ]
		then
			echo -e "    \033[31m $ret \033[0m"		
		else
			echo -e "    \033[32m $ret \033[0m"
			convert_result=1
		fi
	else
		echo "    Is UTF-8 text, no need convert."	
		echo -e "    \033[32m $ret \033[0m"
		convert_result=1
	fi
	echo -e "-----------------------------------------------------------------------------\n"
	return $convert_result
}

#参数1是文件名，文件内容为要转换编码的文件名列表
function GBK2UTF8ByFileList()
{
	local convert_list="$1"
	local convert_success=0
	local convert_failed=0
	local file_cnt=`wc -l "$convert_list"`
	local conv_i=1
	if [ -z "$convert_list" ]
	then
		return
	fi
	
	while read -r line
	do
		#echo "line:$line"
		filename=`echo "$line" | awk -F ':' '{print $1}'`
		echo "($conv_i/$file_cnt) convert file:$filename"
		conv_i=$((conv_i+1))
		GBK2UTF8 "$filename" "$filename"1
	done < $convert_list
}


file_list="$arg1"
echo "file list:$file_list"
GBK2UTF8ByFileList ${file_list}

echo "Convert finish!"
