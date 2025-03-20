<div align="center">
  <img src="https://www.lingo-db.com/images/lingodb-black-title.png" height="50">
</div>
<p>&nbsp;</p>
<p align="center">
  <a href="https://github.com/lingo-db/lingo-db/actions">
    <img src="https://github.com/lingo-db/lingo-db/actions/workflows/workflow.yml/badge.svg?branch=main" alt="Github Actions Badge">
  </a>
  <a href="https://codecov.io/gh/lingo-db/lingo-db" >
    <img src="https://codecov.io/gh/lingo-db/lingo-db/branch/main/graph/badge.svg?token=7RC3UD5YEA"/>
  </a>
</p>

# LingoDB
LingoDB is a cutting-edge data processing system that leverages compiler technology to achieve unprecedented flexibility and extensibility without sacrificing performance. It supports a wide range of data-processing workflows beyond relational SQL queries, thanks to declarative sub-operators. Furthermore, LingoDB can perform cross-domain optimization by interleaving optimization passes of different domains and its flexibility enables sustainable support for heterogeneous hardware.

# Using LingoDB
You can try out LingoDB through different ways:
1. Use the hosted [SQL Webinterface](https://www.lingo-db.com/interface/)
2. Use the python package: `pip install lingodb`
3. Build it yourself by following the [documentation](https://www.lingo-db.com/docs/gettingstarted/install/#building-from-source)

# Using LingoDB's Arrow Flight SQL
## Docker (recommended)
Build Dockerfile located in tools/docker/server
```bash
docker build tools/docker/server -t lingodbserver:latest
```
Run docker container 

```bash
docker build tools/docker/server -t lingodbserver:latest
```
## Building 
### Dependencies 
To use the Flight SQL server you specific need Apache **Arrow 19.x, Arrow Flight 19.x and Arrow Flight SQL 19.x**
How to install them, you can find out here: https://arrow.apache.org/install/ 
### Building 
Inside the root directory of the project execute:
```bash
make build-release 
```
### Executing
```bash
./build/lingodb-release/server <port> <databaseName>:<databaseFolder>
```
You can then connect the server using the Apache Arrow JDBC Driver:
```bash
jdbc:arrow-flight-sql://<ip>:<port>?useEncryption=false
```

## Documentation
For LingoDB's documentation, please visit [the documentation website](https://www.lingo-db.com/docs/) based on [this github repo](https://github.com/lingo-db/lingo-db.github.io).

## Contributing
Before contributing, please first read the [contribution guidelines](https://www.lingo-db.com/docs/next/ForDevelopers/Contributing).