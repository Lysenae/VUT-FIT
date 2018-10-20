#!/bin/bash

TEMP_F=$(mktemp /tmp/$xklima22.XXXXXX)
GSWITCH=false
RSWITCH=false
DSWITCH=false
OBJ_ID=false
declare -a FILES

trap "{ rm -f $TEMP_F; }" SIGINT SIGTERM SIGHUP

function print_dependancies
{
  if [ $1 == false ]; then
    printf "%s -> %s (%s)\n" $2 $3 $4
  else
    o1=$( echo $2 | sed 's/\./D/')
    o2=$( echo $3 | sed 's/\./D/')
    printf "%s -> %s [label=\"%s\"];\n" $o1 $o2 $4
  fi
}

function get_dependancies
{
  for file in $FILES
  do
    echo "Name "$file
    nm $file
    echo
  done > $TEMP_F

  declare -a has
  declare -a requires

  ri=0
  hi=0
  o_file=
  d_req=
  d_has=

  while read line
  do
    if [[ -n $line ]]; then
      check=$(echo $line | awk '$1 == "Name" {print $2}')
      if [[ -n $check ]]; then
        o_file=$check
      else
        check=$(echo $line | awk '{if(length($1)==1) print $1;}')
        if [[ -n $check ]]; then
          if [[ $check == "U" ]]; then
            sym=$(echo $line | awk '{print $2}')
            if [[ -n $sym ]]; then
              ri=`expr $ri + 1`
              requires[$ri]=$o_file";"$sym
            fi
          fi
        else
          check=$(echo $line | awk '{if(length($2)==1) print $2;}')
          if [[ -n $check ]]; then
            if [[ $check == "T" ]] || [[ $check == "B" ]]; then
              sym=$(echo $line | awk '{print $3}')
              if [[ -n $sym ]]; then
                hi=`expr $hi + 1`
                has[$hi]=$o_file";"$sym
              fi
            fi
          fi
        fi
      fi
    fi
  done < $TEMP_F

  declare -a deps
  di=0

  for i in ${requires[@]}
  do
    o_file=$(echo $i | awk -F ";" '{print $1;}')
    r_func=$(echo $i | awk -F ";" '{print $2;}')

    for j in ${has[@]}
    do
      s_file=$(echo $j | awk -F ";" '{print $1;}')
      h_func=$(echo $j | awk -F ";" '{print $2;}')

      if [[ $o_file != $s_file ]]; then
        if [[ $r_func == $h_func ]]; then
          di=`expr $di + 1`
          deps[$di]=$o_file";"$s_file";"$r_func
        fi
      fi
    done
  done

  if [[ $GSWITCH == false ]]; then
    for i in ${deps[@]}
    do
      obj1=$(echo $i | awk -F ";" '{print $1;}')
      obj2=$(echo $i | awk -F ";" '{print $2;}')
      sym=$(echo $i | awk -F ";" '{print $3;}')

      if [[ $DSWITCH == true ]]; then
        if [[ $obj1 == $OBJ_ID ]]; then
          print_dependancies $GSWITCH $obj1 $obj2 $sym
        fi
      elif [[ $RSWITCH == true ]]; then
        if [[ $obj2 == $OBJ_ID ]]; then
          print_dependancies $GSWITCH $obj1 $obj2 $sym
        fi
      else
        print_dependancies $GSWITCH $obj1 $obj2 $sym
      fi
    done
  else
    echo "digraph GSYM {"
    for i in ${deps[@]}
    do
      obj1=$(echo $i | awk -F ";" '{print $1;}')
      obj2=$(echo $i | awk -F ";" '{print $2;}')
      sym=$(echo $i | awk -F ";" '{print $3;}')

      if [[ $DSWITCH == true ]]; then
        if [[ $obj1 == $OBJ_ID ]]; then
          print_dependancies $GSWITCH $obj1 $obj2 $sym
        fi
      elif [[ $RSWITCH == true ]]; then
        if [[ $obj2 == $OBJ_ID ]]; then
          print_dependancies $GSWITCH $obj1 $obj2 $sym
        fi
      else
        print_dependancies $GSWITCH $obj1 $obj2 $sym
      fi
    done
    for file in $FILES
    do
      fsub=$( echo $file | sed 's/\./D/')
      printf "%s [label=\"%s\"];\n" $fsub $file
    done
    echo "}"
  fi
}


while getopts "gr:d:" opt
do
  case $opt in
    g) GSWITCH=true;;
    r)
      if [[ $DSWITCH != true ]]; then
        RSWITCH=true
      else
        continue
      fi
      if [[ -z $OBJ_ID ]]; then
        OBJ_ID=$OPTARG
      fi;;
    d)
      if [[ $RSWITCH != true ]]; then
        DSWITCH=true
      else
        continue
      fi
      if [[ -z $OBJ_ID ]]; then
        OBJ_ID=$OPTARG
      fi;;
    ?)
      exit 1
      ;;
    :)
      exit 1
      ;;
  esac
done

shift $((OPTIND-1))

FILES=$@

get_dependancies

rm -f $TEMP_F
