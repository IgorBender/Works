#!/bin/bash

if [ $# -ne 1 ]; then
   echo "Usage : $0 <path>"
   exit 1
fi

cd $1
mkdir ../for_export
pwd_var=`pwd`
dir_name=`basename $pwd_var`
cp -r ../$dir_name ../for_export
cd ../for_export/$dir_name
for m in `find . -name \*.pro.user.\*`
do
	rm $m
done
for f in `find . -name \*.pro.user`
do
	pwd_var2=`pwd`
	cd "${f%/*}"
	mm=${f##*/}
	nn=${mm%.*}
	mv $mm $nn.shared
	sed -i -- 's/>18</>10</g' $nn.shared
	cd $pwd_var2
done
cd ../
tar cf $dir_name.tar.gz $dir_name
mv $dir_name.tar.gz $pwd_var
cd $pwd_var
mv $dir_name.tar.gz ../
cd ../
rm -r for_export






