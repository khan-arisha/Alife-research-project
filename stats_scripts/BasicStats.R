require(ggplot2)
require(scales)

#These are attractive and color-blind friendly colors to use
#Could also use the viridis package: https://cran.r-project.org/web/packages/viridis/vignettes/intro-to-viridis.html
colors <- c("#673F03", "#B50142","#AB08FF","#7B1DFF", "#5731FD","#5E8EFF", "#4755FF" ,"#6FC4FE", "#86E9FE", "#96FFF7", "#B2FCE3", "#BBFFDB", "#D4FFDD", "#EFFDF0")

#Set your working directory
# TODO: Change PATH_TO_YOUR_FOLDER
setwd("~/CS361")

#You can have whatever variables you want like this
pop_cap <- 10000

#Read in your data
data50 <- read.table("pop50.data", sep = ",")
data100 <- read.table("pop100.data", sep = ",")
data500 <- read.table("pop500.data", sep = ",")
data1000 <- read.table("pop1000.data", sep = ",")


colnames(data50) <- data50[1,]
data50 <- data50[-1,]
colnames(data100) <- data100[1,]
data100 <- data100[-1,]
colnames(data500) <- data500[1,]
data500 <- data500[-1,]
colnames(data1000) <- data1000[1,]
data1000 <- data1000[-1,]

#creating data frame
population <- c(50, 100, 500, 1000)
fitness <- c(data50$mean_fitness[40], data100$mean_fitness[40], data500$mean_fitness[40], data1000$mean_fitness[40])
intitial_data$sd<- c(sd(data50$mean_fitness), sd(data100$mean_fitness), sd(data500$mean_fitness), sd(data1000$mean_fitness))

mutation_rate <- c(0.02,0.02,0.02,0.02)
tasks <- c(1,1,1,1)
intitial_data <- data.frame(population, fitness, mutation_rate,tasks )

intitial_data$fitness <- as.numeric(intitial_data$fitness)

intitial_data$population <-as.numeric(intitial_data$population)
#graph the data
graph1 <- ggplot(intitial_data,  aes(x=population, y=fitness)) + geom_point() + geom_line()  +
  labs(y = "Average Fitness" , x= "Initial Population")

####### Mutation rate 0.002 plot

`50mu0002` <- read.table("50mu0002.data", sep = ",")
`100mu0002` <- read.table("100mu0002.data", sep = ",")
`500mu0002` <- read.table("500mu0002.data", sep = ",")
`1000mu0002` <- read.table("1000mu0002.data", sep = ",")


colnames(`50mu0002`) <- `50mu0002`[1,]
`50mu0002` <- `50mu0002`[-1,]
colnames(`100mu0002`) <- `100mu0002`[1,]
`100mu0002` <- `100mu0002`[-1,]
colnames(`500mu0002`) <- `500mu0002`[1,]
`500mu0002` <- `500mu0002`[-1,]
colnames(`1000mu0002`) <- `1000mu0002`[1,]
`1000mu0002` <- `1000mu0002`[-1,]


population <- c(50, 100, 500, 1000)
fitness <- c(`50mu0002`$mean_fitness[40], `100mu0002`$mean_fitness[40], `500mu0002`$mean_fitness[40], `1000mu0002`$mean_fitness[40])
#intitial_data$sd<- c(sd(data50$mean_fitness), sd(data100$mean_fitness), sd(data500$mean_fitness), sd(data1000$mean_fitness))

mutation_rate <- c(0.002,0.002,0.002,0.002)
mu_data <- data.frame(population, fitness, mutation_rate )

mu_data$fitness <- as.numeric(mu_data$fitness)

mu_data$population <-as.numeric(mu_data$population)
#total <- merge(intitial_data,mu_data,by=c("population"))
#graph the data
graph2 <-  ggplot(mu_data,  aes(x=population, y=fitness)) + 
  geom_point() + geom_line() +
  labs(y = "Average Fitness" , x= "Initial Population")


total <- merge(intitial_data,mu_data,by=c("population"))
#graph the data
graph3 <-  ggplot(total) + geom_point( aes(x=population, y=fitness.x)) + 
  geom_point(aes(x=population, y=fitness.y)) + geom_line(aes(x=population, y=fitness.x, color = as.factor(mutation_rate.x))) +
  geom_line(aes(x=population, y=fitness.y, color = as.factor(mutation_rate.y))) +
  labs(y = "Average Fitness" , x= "Initial Population", color = "Mutation Rate")


####### 4 Tasks

task50 <- read.table("task50.data", sep = ",")
task100 <- read.table("task100.data", sep = ",")
task500 <- read.table("task500.data", sep = ",")
task1000 <- read.table("task1000.data", sep = ",")


colnames(task50) <- task50[1,]
task50 <- task50[-1,]
colnames(task100) <- task100[1,]
task100 <- task100[-1,]
colnames(task500) <- task500[1,]
task500 <- task500[-1,]
colnames(task1000) <- task1000[1,]
task1000 <- task1000[-1,]



fitness <- c(task50$mean_fitness[40], task100$mean_fitness[40], task500$mean_fitness[40], task1000$mean_fitness[40])
#intitial_data$sd<- c(sd(data50$mean_fitness), sd(data100$mean_fitness), sd(data500$mean_fitness), sd(data1000$mean_fitness))

mutation_rate <- c(0.02,0.02,0.02,0.02)
tasks <- c(4,4,4,4)
tasks_data <- data.frame(population, fitness, mutation_rate, tasks )

tasks_data$fitness <- as.numeric(tasks_data$fitness)

tasks_data$population <-as.numeric(tasks_data$population)
#graph the data
graph4 <-  ggplot(tasks_data,  aes(x=population, y=fitness)) + 
  geom_point() + geom_line() +
  labs(y = "Average Fitness" , x = "Initial Population")


total <- merge(intitial_data , tasks_data,by=c("population"))
#graph the data
graph5 <-  ggplot(total) + geom_point( aes(x=population, y=fitness.x)) + 
  geom_point(aes(x=population, y=fitness.y)) + geom_line(aes(x=population, y=fitness.x, color = as.factor(tasks.x))) +
  geom_line(aes(x=population, y=fitness.y, color = as.factor(tasks.y))) +
  labs(y = "Average Fitness" , x= "Initial Population", color = "Number of tasks")



