cd out
rm *
cd ..
cd out2
rm *
cd ..
python fuse.py $1
python present.py
