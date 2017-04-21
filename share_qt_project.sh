#!/bin/bash

for f in `find . -name \*.pro.user`
do
	pwd_var2=`pwd`
        if ! [ -d $pwd_var2/export ]; then
           mkdir $pwd_var2/export
        fi
	cd "${f%/*}"
	mm=${f##*/}
	nn=${mm%.*}
        if [ -f $pwd_var2/export/$nn.shared ]; then
            if [[ $mm -nt $pwd_var2/export/$nn.shared ]]; then
                cp $mm $pwd_var2/export/$nn.shared
                sed -i -- 's/>18</>10</g' $pwd_var2/export/$nn.shared
            fi
        else
            cp $mm $pwd_var2/export/$nn.shared
            sed -i -- 's/>18</>10</g' $pwd_var2/export/$nn.shared
        fi
	cd $pwd_var2
done
