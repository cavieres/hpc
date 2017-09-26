library(ggplot2)

setwd("C:\\Users\\cavie\\Documents\\Estudios\\USACh\\II Sem\\HPC - Computación de Alto Rendimiento\\hpc-github\\lab-2\\Informe")

df = read.csv("lab2-benchmark-dell-precision-tower-3620-8-cores-dot.csv", header = TRUE, sep = ";")

bm128 = subset(df, ï..N == 128)

# Speedup Relativo
ggplot(subset(bm128, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Srel,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Speedup relativo")

# Eficiencia
ggplot(subset(bm128, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Eff,
           color=T))+
  geom_line(aes(color = T), size = 1)  + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Eficiencia")

# Tiempo
ggplot(subset(bm128, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Time,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Tiempo")


bm256 = subset(df, ï..N == 256)

# Speedup Relativo
ggplot(subset(bm256, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Srel,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Speedup relativo")

# Eficiencia
ggplot(subset(bm256, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Eff,
           color=T))+
  geom_line(aes(color = T), size = 1)  + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Eficiencia")

# Tiempo
ggplot(subset(bm256, T %in% c("2K", "4K", "8K")),
       aes(x=H,
           y=Time,
           color=T))+
  geom_line(aes(color = T), size = 1) + facet_wrap(~T, ncol = 3) + labs(x = "Hebras", y = "Tiempo")
