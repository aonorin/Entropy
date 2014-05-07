#! /bin/bash
sed -i '' 's/ -Xarch_x86_64/ /g' $1
sed -i '' 's/ -arch x86_64/ /g' $1