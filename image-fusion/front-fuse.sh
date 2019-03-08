cd out
rm *
cd ..
cd out2
rm *
cd ..
python front-fuse-draft.py $1
python present.py
