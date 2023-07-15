# Script Structure

```
main
├── db
│   ├── create-database
│   │   ├── create-table
│   │   ├── create-column
│   │   ├── insert-row
│   │   ├── update-row
│   │   ├── delete-row
│   │   ├── select-row (query)
│   │  
│   ├── view-databases
│   ├── use-database
│   │  
│   
├── version
├── help
├── about
├── exit

```

# Script

```
// for version check
main -v
main --version
```

```
// for help
main --help
main -h
```

```
// for about
main --about
```

```
// for create database name `defaultdb`
main db create
// for create database name user defined
main db create --name <database-name>

```
