#!/bin/bash

create_dir()
{
    if ! [ -d $1 ]; then
        mkdir $1
    fi
}

build_f ()
{
   create_dir $1
   create_dir $2

   if [ -f $3 ]; then
       cp $3 $2
   fi
   if [ -f lib$3.so ]; then
       cp lib$3.so $2
   fi
   if [ -f lib$3.a ]; then
       cp lib$3.a $2
   fi
}

clean_f()
{
    if [ -f $2/$3 ]; then
        rm $2/$3
    fi
    if [ -f $2/lib$3.so ]; then
        rm $2/lib$3.so
    fi
    if [ -f $2/lib$3.a ]; then
        rm $2/lib$3.a
    fi
}

if [ $4 = "clean" ]; then
    clean_f $1 $2 $3
else
    build_f $1 $2 $3
fi
