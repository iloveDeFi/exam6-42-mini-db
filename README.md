# mini_db

## 👋 Hi there
Here is the new 2025 exam6 subject called: mini_db. Now you can train properly before passing the exam. Good luck and have fun!

## Overview
**mini_db** is a project for the **Exam Rank 6** at 42. The goal is to create a simple, performant database server that listens for client connections, processes commands, and maintains a key-value database. The database must persist data between program executions using a specified file.

---

## Project Description

The program accepts two arguments:
1. A **port number** to listen on (e.g., 127.0.0.1:port).
2. A **file path** to save and load the database.

The program listens for client connections and handles the following commands:

### Commands

#### **POST**
- **Synopsis:** `POST key value`
- **Description:** Adds a key-value entry to the database.
- **Return Value:**
  - `0` if the operation is successful.

#### **GET**
- **Synopsis:** `GET key`
- **Description:** Retrieves a value for the given key.
- **Return Value:**
  - `0 value` if the key exists.
  - `1` if the key does not exist.

#### **DELETE**
- **Synopsis:** `DELETE key`
- **Description:** Removes a key-value entry from the database.
- **Return Value:**
  - `0` if the key was successfully deleted.
  - `1` if the key does not exist.

#### **Unknown Command**
- **Return Value:**
  - `2` for unrecognized commands.

### Additional Details
- **Key and Value Size:**
  - Keys and values can have arbitrary sizes but must not contain whitespace.
  - Requests will not exceed 1000 characters.

- **Persistent Connections:**
  - Clients can send multiple commands in a single session.

- **Data Persistence:**
  - On receiving the `SIGINT` signal, the program saves the database to the specified file and exits.
  - On startup, the program loads the database from the file if it exists.

---

## Example Usage

### Running the Server
```bash
$> ./mini_db 1111 .save &
```

### Connecting with `nc` (Netcat)
```bash
$> nc localhost 1111
```

### Example Commands and Output
```bash
POST A B
0
POST B C
0
GET A
0 B
GET C
1
DELETE A
0
DELETE C
1
UNKNOWN_COMMAND
2
```

### Saving the Database
Send the `SIGINT` signal to save the database:
```bash
$> pkill -SIGINT mini_db
```

### Restarting the Server with Saved Data
```bash
$> ./mini_db 1111 .save &
$> echo "GET B" | nc localhost 1111
0 C
```

---

## Requirements
- **Input Handling:**
  - Handle up to 1000 characters per request.
  - Ensure performance and low latency.

- **Network Protocol:**
  - Use `127.0.0.1` and the given port for client connections.

- **Error Handling:**
  - Gracefully handle invalid commands.

---

## Testing
You can use `nc` (Netcat) to simulate client connections and test commands:
```bash
$> nc localhost <port>
```

---

## Provided Files
This project includes some starter network code to help manage sockets. Ensure you understand and properly utilize it.

---

## Example Code Structure
The project includes the following classes:

### **Socket**
Handles low-level socket creation, binding, listening, and accepting clients.

### **Server**
Encapsulates server logic and runs the main event loop to process commands from connected clients.

---

## Hints
- Use `SIGINT` for graceful shutdown and database persistence.
- Test your implementation thoroughly with multiple clients and edge cases.
- Ensure your program efficiently handles large numbers of requests.

---

**Good luck!**

