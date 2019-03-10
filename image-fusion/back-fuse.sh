cd out
rm *
cd ..
cd out2
rm *
cd ..
python back-fuse-draft.py $1
python compare.py
