python3 factory-sounds-to-soundmap.py $1 > _temp.xml
xmllint --format _temp.xml > _out.xml
rm _temp.xml
