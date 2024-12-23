declare -a arr=("uniao" "interseccao_nula" "pertence" "remocao" "ambos_nulos")

## now loop through the above array
for i in "${arr[@]}"
do
   if ! diff <(./programa < "$i.in") "$i.out"; then
   	echo "problema em $i"
   fi
   if ! diff <(./programa < "$i (copy).in") "$i (copy).out"; then
   	echo "problema em $i"
   fi
done

