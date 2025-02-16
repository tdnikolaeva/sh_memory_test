# sh_memory_test

## Состав репозитория

*   `src/`: Содержит исходные файлы программ `prog1.c` и `prog2.c`.
*   `Makefile`: Файл для сборки проекта.
*   `Dockerfile`: Файл для создания Docker-образа.
*   `README.md`: Файл, содержащий инструкции по сборке и запуску.


## Сборка и запуск через докер

```bash
cd sh_memory_test
docker build -t shmemory_test .
docker run --name shmem_container shmemory_test
```

## Сборка и запуск непосредственно на своем устройстве

```bash
cd sh_memory_test
make
./prog1 | ./prog2
```

## Очистка при использовании докера

```bash
docker rm shmem_container
docker rmi shmemory_test
```

## Очистка при запуске на своем устройстве

```bash
make clean
```
