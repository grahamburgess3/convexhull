# supplementary functions to the convex_hull package
in_hull <- function(points, hull){
  #Checks whether any of the suggested points are on the convex hull. 
  #Input: 
  #  - points: list(c(x_0,y_0),c(x_1,y_1),...)
  #  - hull: list(c(x_0,y_0,x_1,y_1),c(x_1,y_1,_x_2,y_2),...)
  #Return: 
  #  - output: list(c(x_0,y_0),c(x_1,y_1),...) the points which are in the hull. 
  
  # how many points
  n <- length(points)
  
  # check if in hull
  inhull <- c()
  for (i in 1:n){
    inhull[i] <- check(i, points, hull)
  }
  
  # output
  output <- list()
  j <- 1 # counter of points which are on hull
  for (i in 1:n){
    if (inhull[i] == TRUE){
      output[[j]] <- points[[i]]
      j <- j + 1
    }
  }
  if (length(output)==0){
    output <- "None of the suggested points lie on the convex hull"
  }
  
  # return
  return(output)
}

# checking function
check <- function(i, points, hull){
  #Checks whether the ith point is in the convex hull
  #Returns: bool

  for (j in 1:length(hull)){
    if (points[[i]][1] == hull[[j]][1] & points[[i]][2] == hull[[j]][2]){
      return(TRUE)
    }
  }
  return(FALSE)
}
