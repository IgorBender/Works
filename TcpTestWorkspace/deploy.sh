#!/bin/bash

# $1 is workspace directory
# $2 is delployment directory

debug_name="Debug"
release_name="Release"

create_dir()
{
    if ! [ -d $1 ]; then
        mkdir $1
    fi
}

deploy_f()
{
# $1 - configuration directory
# $2 - delployment directory
# $3 - project name
    create_dir $2
    create_dir $2/$1
    if [ -f $3/$1/$3 ]; then
       cp $3/$1/$3 $2/$1
    else
       if [ -f $2/$1/$3 ]; then
           rm $2/$1/$3
       fi
    fi
    if [ -f $3/$1/lib$3.so ]; then
       cp $3/$1/lib$3.so $2/$1
    else
       if [ -f $2/$1/lib$3.so ]; then
           rm $2/$1/lib$3.so
       fi
    fi
    if [ -f $3/$1/lib$3.a ]; then
       cp $3/$1/lib$3.a $2/$1
    else
       if [ -f $2/$1/lib$3.a ]; then
       	   rm $2/$1/lib$3.a
       fi
    fi
}

echo $d
for d in $(ls $1)
do
    for dd in $(ls $d)
    do
     if [ -d $d/$dd ]; then
	dirname=$dd
	if [ $debug_name = ${dirname:0:${#debug_name}} ]; then
		deploy_f $dirname $2 $d
 	fi
 	if [ $release_name = ${dirname:0:${#release_name}} ]; then
		deploy_f $dirname $2 $d
	fi
    fi
    done
done

