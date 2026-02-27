# Project3-LB-CSCE412

## Building and Running

Compile and run the project using:
```bash
make && ./loadbalancer
```

## Command Line Arguments

The program accepts optional command line arguments:

```bash
./loadbalancer [numServers] [clockCycles]
```

### Parameters:
- **numServers** (optional): Number of web servers to start with
  - Default: `10`
- **clockCycles** (optional): Number of clock cycles to run the simulation
  - Default: `10000`

### Usage Examples:

Run with default settings (10 servers, 10000 cycles):
```bash
./loadbalancer
```

Run with 5 servers and default simulation time:
```bash
./loadbalancer 5
```

Run with 5 servers for 5000 clock cycles:
```bash
./loadbalancer 5 5000
```
