# Hello !
### Описание работы методов класса "InvertedIndex"
<h5>Метод "GetWordCount"</h5>
принимает слово word которое надо найти в документе<br>
возвращает структуру Entry с результатами <br>
содержит vector<Entry> bufferEntry - буффер для хранения результатов поиска.<br>
основной цикл for который переключает нас с документа на документ. и вложенный в него цикл while
<p>внутри цикла for :</p>
size_t count = 0 - кол-во слов в документе<br>
stringstream currentStream(docs[id]) - файловый поток с текущим документом<br>
string wordFromStream - текущее слово из потока, которое мы проверяем<br>
<p>вложенный цикл while</p>
тут мы сверяем полученное слово со словами из документа и если находим увеличивает count.<br>
<p>по завершении работы цикла while добавляем в буффер запись с результатами.</p>