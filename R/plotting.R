# plot function
plot_hull <- function(data){
  #Plots the set of points and the convex hull
  #Input: data: list(input_data, hull)
  #Returns: a ggplot object
  
  # get data
  points <- data$input_data
  hull <- data$hull
  
  # get libs
  library(ggplot2)
  
  # create points dataframe
  X <- c()
  Y <- c()
  point_type <- c()
  for (pair in points){
    X <- c(X, pair[1])
    Y <- c(Y, pair[2])
    point_type <- c(point_type, "interior points")
  }
  df_points <- data.frame(X,Y,point_type)
 
  # create hull dataframe
  X <- c()
  Y <- c()
  point_type <- c()
  for (pair in hull){
    X <- c(X, pair[1])
    Y <- c(Y, pair[2])
    point_type <- c(point_type, "convex hull")
  }
  df_hull <- data.frame(X,Y,point_type)

  # plot
  df <- rbind(df_points, df_hull)
  p <- ggplot(df, aes(x = X, y = Y)) +
    geom_point(aes(color = point_type)) +
    geom_polygon(data = df_hull, alpha=0.2) +
    ggtitle("Convex hull and interior points") +
    theme_bw()
    
  return(p)
}
