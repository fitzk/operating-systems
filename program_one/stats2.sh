#!/bin/bash 

set -u # treat unset variables as an error, exit immediately 
set -e # exit script if any statement returns a non true value
set -euo pipefail # causes a pipeline to produce a failure return code if any command errors (instead of just if the last command errors)



declare -a BIG_ARRAY   ## stores the contents of the input stream/file, one row per index ##
declare -a AVE_ARRAY    ## stores the averages as they are calculated ##
declare -a MED_ARRAY    ## stores the medians as they are calculated ##

##########################################
##                          print_col_stats                                  ##
##  parameters:  none                                                    ##
##  behavior:  prints the formatted array of                  ##
##  averages and medians to standard output            ##
##########################################
function print_col_stats(){

    printf "Averages:\n"
    for ave in "${AVE_ARRAY[@]}";do
        printf "%s\t" "$ave"
    done
    printf "\nMedians:\n"
    for med in "${MED_ARRAY[@]}";do
        printf "%s\t" "$med"
    done  
    printf "\n"

}
##########################################
##                          print_row_stats                                  ##
##  parameters:  none                                                    ##
##  behavior:  prints the formatted array of                  ##
##  averages and medians to standard output            ##
##########################################
function print_row_stats(){

    declare -a ave_array
    declare -a med_array
    printf "Average Median\n"
    for (( idx=0 ; idx < "${#AVE_ARRAY[*]}" ; idx++ ));do 
        printf "%s\t%s\n" "${AVE_ARRAY[$idx]}" "${MED_ARRAY[$idx]}"
    done
    

}
##########################################
##                                      median                                  ##
##  parameters:  an array of numbers                          ##
##  behavior:  returns the median of the input array    ##
##########################################
function median() {
##  I found a couple of ways to pass arrays as arguements
## to functions, but the method of indirect parameter expansion
## seemed to be the syntactically simplest way to do it. I read
## that an array has access to a local array declared in the calling 
## function, but I wasn't sure if it was worth changing the way the
## parameters are accessed if it got me the same result. Please
## let me know if there is a prefered method of passing/accessing
## arrays as arguements :)
    declare -a line=( "${!1}" )
    local length="${#line[@]}"  ## storing the number of nums
    #  'Here Strings' <<< used in the following lines to 
    # sort the array would not work with the default ifs
   oIFS=$IFS    
    IFS=$'\n'       
    local sorted=($(sort <<< "${line[*]}"))    ## sorting and storing
    local middle
    (( middle=length/2 ))
    [[ "expr $length % 2" == 0 ]] && (( middle+=1 )) ## checking for even or odd
    echo "${sorted[$middle]}"
    unset sorted
    unset sum
    unset line
    unset middle
   IFS=$oIFS
}
##########################################
##                                      average                                 ##
##  parameters:  an array of numbers                          ##
##  behavior:  returns the average of the input array  ##
##########################################
function average() {

    declare -a line=( "${!1}" )
    local length="${#line[@]}"
    if [ "$length" -gt 0 ]; then
        local sum=0
        local ave=0
        for num in "${line[@]}"; do
            (( sum+=num ))
        done
        (( ave=sum+length-1 ))
        (( ave=ave/length ))
       echo "$ave"
    fi
    unset ave
    unset length
    unset sum
    unset line
}
##########################################
##                                      row_stats                               ##
##  parameters:  an array of numbers                          ##
##  behavior:  returns the average of the input array  ##
##########################################
function row_stats() {

    declare -a rows=("${!1}")  

    for row in "${rows[@]}"; do
        read -a row_array <<< "$row"
         local length="${#row_array[@]}"
          
        if [ "$length" -gt 0 ]; then
            local ave=$(average row_array[@])
            AVE_ARRAY+=("$ave")
             local med=$(median row_array[@])
            MED_ARRAY+=("$med")
         fi
         unset ave med
    unset row_array
    unset rows
   done
   
 
}

function column_stats(){
    declare -a rows=("${!1}")
    declare -a column
    declare -a row_array
    
    read -a row_array <<< "${rows[0]}"
    local length="${#row_array[@]}" 
    
    for (( idx=0 ; idx<length ; idx++ )); do 
       for row in "${rows[@]}"; do
            read -a row_array <<< "$row"
            [[ "${#row_array[@]}" -gt 0 ]] && column+=( "${row_array[$idx]}" )         
      done
      
      local ave=$(average column[@]);
      AVE_ARRAY+=("$ave")
      local med=$(median column[@])
      MED_ARRAY+=("$med")
       unset column
       unset row_array
   done
}



## called with just stats ##
if [ "$#" -eq 0 ];then
     echo "Usage: stats {-rows|-cols} [file]"  >&2
     exit 1
fi
    if [ "$#" -ge 3 ]; then
         echo "Usage: stats {-rows|-cols} [file]"  >&2
     exit 1
    fi

## check for command 




if [ ! -z "$(echo $1 | egrep '^-[r,c].*')" ]; then
    key="$(echo $1 | egrep '^-[r,c].*'| cut -c1-2)"
else
    key=3
fi
if [ "$#" -eq 2 ]; then    
   
   if [ ! -f "$2" ]; then
        echo "stats: cannot read $2" >&2
        exit 1
    elif [ ! -r "$2" ]; then
        echo "stats: cannot read $2" >&2
        exit 1
    elif [ ! -e "$2" ]; then 
    echo "stats: cannot read $2" >&2
        exit 1
    else
    FILE="$2"
        while read line; do 
            BIG_ARRAY+=("$line")		
        done<$FILE
     fi
else
    while read line; do 
        BIG_ARRAY+=("$line")		
    done 
fi


case $key in
     '-r')

      row_stats BIG_ARRAY[@]
      print_row_stats AVE_ARRAY[@] MED_ARRAY[@]
      shift
	 ;;
     '-c')
    column_stats BIG_ARRAY[@]
    print_col_stats AVE_ARRAY[@] MED_ARRAY[@]
	 shift
	 ;;
     
     3)
     echo "Usage: stats {-rows|-cols} [file]"  >&2
     exit 1
     shift
     ;;
 esac	
exit 0

	


