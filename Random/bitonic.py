# Python3 program to check if 
# an array is bitonic or not.
 
# Function to check if the 
# given array is bitonic
def checkBitonic(arr, n) :
 
    # Check for increasing sequence
    for i in range(1, n) :
        if arr[i] > arr[i - 1] :
            continue
        else :
            break
 
    if i == n-1 :
        return 1
 
    # Check for decreasing sequence
    for j in range(i + 1, n) :
         
        if arr[j] < arr[j - 1] :
            continue
        else :
            break
 
    i = j
    if i != n - 1 :
        return 0
 
    return 1
 
# Driver Code
if __name__ == "__main__" :
     
    arr = [1,4,2,3]
     
    n = len(arr)
 
    if checkBitonic(arr, n) == 1 :
        print("YES")
    else :
        print("NO")
 
# This code is contributed
# by ANKITRAI1 