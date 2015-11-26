teststring="test1,test2,test3,test4,test5,test6"
array1=( ${teststring//,/ } )

num=$(( ${#array1[@]} -1 ))
newseq=$( shuf -i 0-$num )

for i in $newseq; do

     outstring+="${array1[$i]},"

done

echo ${outstring%,}