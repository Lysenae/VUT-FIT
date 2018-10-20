#!/usr/bin/env bash

TEMP_F=$(mktemp /tmp/$USER.XXXXXX)
GSWITCH=false
PSWITCH=false
RSWITCH=false
DSWITCH=false
FUNC_ID=
IN_FILE=

trap "{ rm -f $TEMP_F; }" SIGINT SIGTERM SIGHUP

function print_err
{
  echo "$@" 1>&2
}

function find_func_as_caller
{
  declare -a fces
  i=0
  func_found=false
  while read line
  do
    if [[ -n $line ]]; then
      fcaller=$(echo $line | awk '$3 ~ /^[ ]*$/ { print $2 }')
      if [[ -z $fcaller ]]; then
        if [[ $func_found == true ]]; then
          callq=""
          if [ $2 == false ]; then
            callq=$(echo $line | awk '$7 == "callq" && !($9 ~ /\w+@\w+/) \
              { print $9 }')
          else
            callq=$(echo $line | awk '$7 == "callq" { print $9 }')
          fi
          if [[ -n $callq ]]; then
            callq=$(echo $callq | sed 's/<//' | sed 's/>//' | sed 's/\+.*//')
            echo $callq
            i=`expr $i + 1`
            fces[$i]=$callq
          fi
        fi
      else
        func_id=$(echo $fcaller | sed 's/<//' | sed 's/>://')
        if [ $func_id == $1 ]; then
          func_found=true
        else
          if [[ $func_found == true ]]; then
            func_found=false
          fi
        fi
      fi
    fi
  done < $TEMP_F
  echo "${fces[@]}"
}

function find_func_as_callee
{
  declare -a fces
  i=0
  fcaller_found=false
  while read line
  do
    if [[ -n $line ]]; then
      check=$(echo $line | awk '$3 ~ /^[ ]*$/ { print $2 }')
      if [[ -n $check ]]; then
        fcaller=$(echo $check | sed 's/<//' | sed 's/>://')
        fcaller_found=true
      else
        if [[ $fcaller_found == true ]]; then
          callq=$(echo $line | awk '$7 == "callq" && !($9 ~ /\w+@\w+/) \
            { print $9 }')
          if [[ -n $callq ]]; then
            callq=$(echo $callq | sed 's/<//' | sed 's/>//' | sed 's/\+.*//')
            echo $callq
            if [ $callq == $1 ]; then
              i=`expr $i + 1`
              fces[$i]=$fcaller
              fcaller_found=false
            fi
          fi
        fi
      fi
    else
      fcaller_found=false
    fi
  done < $TEMP_F
  echo "${fces[@]}"
}

function find_all_funcs
{
  declare -a fces
  i=0
  while read line
  do
    if [[ -n $line ]]; then
      check=$(echo $line | awk '$3 ~ /^[ ]*$/ { print $2 }')
      if [[ -n $check ]]; then
        fcaller=$(echo $check | sed 's/<//' | sed 's/>://')
      else
        callq=
        if [ $1 == false ]; then
          callq=$(echo $line | awk '$7 == "callq" && !($9 ~ /\w+@\w+/) \
            { print $9 }')
        else
          callq=$(echo $line | awk '$7 == "callq" { print $9 }')
        fi
        if [[ -n $callq ]]; then
          callq=$(echo $callq | sed 's/<//' | sed 's/>//' | sed 's/\+.*//')
          i=`expr $i + 1`
          fces[$i]=$fcaller";"$callq
        fi
      fi
    fi
  done < $TEMP_F
  echo "${fces[@]}"
}

while getopts "gpr:d:" opt
do
  case $opt in
    g) GSWITCH=true;;
    p) PSWITCH=true;;
    r)
      if [[ $DSWITCH != true ]]; then
        RSWITCH=true
      else
        continue
      fi
      if [[ -z $FUNC_ID ]]; then
        FUNC_ID=$OPTARG
      fi;;
    d)
      if [[ $RSWITCH != true ]]; then
        DSWITCH=true
      else
        continue
      fi
      if [[ -z $FUNC_ID ]]; then
        FUNC_ID=$OPTARG
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
IN_FILE=$1

if ! [[ -e $IN_FILE ]]; then
  print_err "Subor $IN_FILE neexistuje"
  exit 1
fi

objdump -d -j .text $IN_FILE > $TEMP_F
declare -a rslt

if [ $DSWITCH == true ]; then
  rslt=$(find_func_as_caller $FUNC_ID $PSWITCH)

  rslt=($(for i in ${rslt[@]}
  do
    echo $i
  done | sort -u) )

  if [ $GSWITCH == false ]; then
    for i in ${rslt[@]}
    do
      printf "%s -> %s\n" $FUNC_ID $i
    done
  else
    echo "digraph CG {"
    for i in ${rslt[@]}
    do
      r=$(echo $i | sed 's/@plt/_PLT/')
      printf "%s -> %s;\n" $FUNC_ID $r
    done
    echo "}"
  fi
fi

if [ $RSWITCH == true ]; then
  rslt=$(find_func_as_callee $FUNC_ID)

  if [ $GSWITCH == false ]; then
    for i in ${rslt[@]}
    do
      printf "%s -> %s\n" $i $FUNC_ID
    done
  else
    echo "digraph CG {"
    for i in ${rslt[@]}
    do
      r=$(echo $i | sed 's/@plt/_PLT/')
      printf "%s -> %s;\n" $r $FUNC_ID
    done
    echo "}"
  fi
fi

if [ $RSWITCH == false ] && [ $DSWITCH == false ]; then
  rslt=$(find_all_funcs $PSWITCH)
  rslt=($(for i in ${rslt[@]}
  do
    echo $i
  done | sort -u ) )

  if [ $GSWITCH == false ]; then
    for i in ${rslt[@]}
    do
      r=$(echo $i | sed 's/;/ -> /')
      echo $r
    done
  else
    echo "digraph CG {"
    for i in ${rslt[@]}
    do
      r=$(echo $i | sed 's/;/ -> /' | sed 's/@plt/_PLT/g')
      echo $r
    done
    echo "}"
  fi
fi

rm -f $TEMP_F
