### Задание 11

Цель – разработать систему запуска задач <b>MapReduce</b>. Не предполагается запуск внешних задач
для отображения (<b>map</b>) и свёртки (<b>reduce</b>) (т. е. на вход принимаем только C++ функции или
функторы, не нужно реализовывать запуск внешних скриптов).


На вход подаётся тестовый файл F, в котором каждая строка содержит адрес электронной почты.
Для дальнейшей обработки не имеет значения, что именно содержат строки – самое важное, что
обработка выполняется построчно.


В качестве дополнительных параметров также указывается количество потоков для запуска
функций отображения (<b>M</b>) и свертки (<b>R</b>) соответственно.


На первом этапе необходимо выполнить разделение исходного файла на секции по числу потоков
отображения <b>M</b>. При этом нужно следить, чтобы границы секций были выровнены по строке. То
есть, каждая секция заканчивалась целой строкой и начиналась с начала новой строки. Таким
образом мы не допустим, чтобы один из email-адресов оказался на границе секции и частично
попал в две секции сразу. Чтение файла на этом этапе недопустимо, за исключением минимально
возможного для выравнивания по строке.


На этапе работы отображения (<b>map</b>) следует запустить <b>M</b> потоков, каждый из которых
обрабатывает свою секцию, полученную после разделения (<b>split</b>) исходного файла. Задача потока
– построчно считать свою секцию и каждую строчку отправить в пользовательский
функциональный объект. Результатом работы такого объекта будет новый список строк для
стадии свертки (<b>reduce</b>). Полученные списки накапливаются в контейнере и затем сортируются.
Для каждой секции получаем свой контейнер с отсортированными результатами.


Как только все потоки отображения будут завершены, необходимо запустить операцию
смешивания (<b>shuffle</b>) и приготовить <b>R</b> контейнеров для будущей свертки. Общая задача на этапе
смешивания – переместить строки из <b>M</b> контейнеров (результат этапа <b>map</b>) в <b>R</b> контейнеров
(входные данные для этапа <b>reduce</b>). При этом сделать это нужно таким образом, чтобы
одинаковые данные попали в один и тот же контейнер для свёртки. Важно, чтобы контейнеры для
свёртки остались отсортированными. Необходимо понять, как реализовать объединение
отсортированных последовательностей.


Как только <b>shuffle</b> будет завершён, должны будут запуститься <b>R</b> потоков для свертки (<b>reduce</b>).
Каждый поток построчно отправляет данные из контейнера в пользовательский функциональный
объект. Результатом работы такого объекта будет список строк, который должен быть сохранен в
файл без какой-либо дальнейшей обработки.
  
  
К моменту завершения работы всех потоков свертки в файловой системе должны сформироваться <b>R</b> файлов с результатами.
  
  
### <i>Требования к реализации</i>
Результатом работы должна стать система запуска задач <b>MapReduce</b>. С помощью этой системы
нужно решить задачу определения минимально возможного префикса, однозначно
идентифицирующего строку. Для этого потребуется написать два функциональных объекта – для
отображения (<b>map</b>) и свертки (<b>reduce</b>).

Порядок запуска:
```
# mapreduce <src> <mnum> <rnum>
```
где:
- <b>src</b> – путь к файлу с исходными данными;
- <b>mnum</b> – количество потоков для работы отображения;
- <b>runm</b> – количество потоков для работы свертки;


### <i>Проверка</i>
Задание считается выполненным успешно, если после установки пакета и запуска с тестовыми
данными вывод соответствует ожидаемому