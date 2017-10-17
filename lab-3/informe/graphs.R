library(ggplot2)

setwd("C:\\Users\\cavie\\Documents\\Estudios\\USACh\\II Sem\\HPC - ComputaciÃ³n de Alto Rendimiento\\hpc-github\\lab-3\\Informe")

# Tiempo (mismo tamaño de bloque, SIN grabado imagen)
df = read.csv("lab3-benchmark-without-raw.csv", header = TRUE, sep = ";")
wc1 <- df[-c(13, 14, 15), ]
ggplot(subset(wc1, T %in% c("3C", "1K", "10K")),
       aes(x=N,
           y=Time,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Grillas (N)", y = "Tiempo")

# Tiempo (mismo tamaño de bloque, CON grabado imagen)
df = read.csv("lab3-benchmark-with-raw.csv", header = TRUE, sep = ";")
ggplot(subset(df, T %in% c("3C", "1K", "10K")),
       aes(x=N,
           y=Time,
           color=T))+
  geom_line(aes(color = T), size = 1)  + facet_wrap(~T, ncol = 3) + labs(x = "Grillas (N)", y = "Tiempo")

# Tiempo (diferente tamaño de bloque, SIN grabado imagen)
df = read.csv("lab3-benchmark-without-raw-diff-blocks.csv", header = TRUE, sep = ";")
ggplot(subset(df, T %in% c("3C", "1K", "10K")),
       aes(x=XxY,
           y=Time,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Bloques (X, Y)", y = "Tiempo")
