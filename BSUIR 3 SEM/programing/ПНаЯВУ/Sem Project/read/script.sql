-- Create a table Book
CREATE TABLE IF NOT EXISTS Book (
    id INTEGER PRIMARY KEY,
    title TEXT,
    author TEXT,
    img BLOB,
    content BLOB
);

-- Create a table Shelf
CREATE TABLE IF NOT EXISTS Shelf (
    id INTEGER PRIMARY KEY,
    naming TEXT
);

-- Create a table many to many
CREATE TABLE IF NOT EXISTS BooksOnShelfes (
    id INTEGER PRIMARY KEY,
    shelf_id INTEGER,
    book_id INTEGER,
    FOREIGN KEY (shelf_id) REFERENCES Shelf(id),
    FOREIGN KEY (book_id) REFERENCES Book(id)
);

-- create shelf (shelf name)
-- INSERT INTO Shelf (naming) VALUES ('fantasy');

-- addBookOnShelf (shelf id, book id)
-- INSERT INTO BooksOnShelfes (shelf_id, book_id) VALUES (1,8);

-- getBooksFromShelf (shelf_id)
-- SELECT Book.id, Book.title, Book.author, Book.img, Book.content FROM Book INNER JOIN BooksOnShelfes ON BooksOnShelfes.book_id = Book.id INNER JOIN Shelf ON Shelf.id = BooksOnShelfes.shelf_id WHERE Shelf.id = 2;

-- deleteBookFromShelf(shelf_id, book_id)
-- DELETE FROM BooksOnShelfes WHERE book_id = ? AND shelf_id = ?

-- deleteShelf(shelf_id)
-- DELETE FROM BooksOnShelfes WHERE shelf_id=2;
-- DELETE FROM Shelf WHERE id=2;