jarvis_march <- function(data){
  """
  This function returns the convex hull of the input data - which may be:
    - a vector (x_0, y_0, x_1, y_1, ...) of x-y values, 
    - a list (c(x_0,y_0),c(x_1,y_1),...) of x-y values or
    - a data.frame(x=c(x_0,x_1,...),y=c(y_0,y_1,...)) of x-y values
  Returns: 
    - the input data in list form and 
    - the convex hull in list formm
  """
  # error check: empty input
  if(typeof(data)=="NULL"){
    return("Error: no input data")
  }
  
  # vector input
	if(typeof(data)=="double"){
		# error checking: must have even number of entries
	  if(length(data) %% 2 != 0){
	    return("Error: unequal amount of x and y data given in input vector")
	  }
	  else {
	    input_data <- list()
	    for (i in 1:(length(data)/2)){
	      new_vec <- c(data[(2*i)-1],data[2*i])
	      input_data[[i]] <- new_vec
	    }
	  }
	} 
  
  # list or data.frame input
  else if (typeof(data)=="list"){
	  
    # list input
    if (class(data)=="list") {
	    # error checking: data must have at least one list element
      if(length(lis)==0){
        return("Error: no input data")
      }
      # error checking: each list element must have two entries (assumed x and y)      
      for (i in 1:(length(data))){
        if (length(data[[i]]) != 2) {
          return("Error: not all elements of input list have exactly two entries")
        }
      }
      input_data <- data
    } 
    
    # data.frame input
	  else if (class(data)=="data.frame"){
	    # error checking: must not be be empty data frame
	    if (length(data)==0) {
	      return("Error: no input data")
	    }
	    # error checking: must have two rows
	    if (length(data) != 2){
	      return("Error: exactly two columns are required in the input data.frame")
	    }
	    input_data <- list()
	    for (i in 1:(nrow(data))){
	      new_vec <- c(data[i,1],data[i,2])
	      input_data[[i]] <- new_vec
	    }
	  }
  }
  hull <- jarvis_march_cpp(input_data)
  output <- list(input_data = input_data, hull = hull)
  return(output)
}