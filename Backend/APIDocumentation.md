# API Documentation

## Get All Test Suites Analytics

### Request Body

```json
{
  "service": "TestCraftService",
  "api": "getAllTestSuitesAnalytics",
  "par": {
    "userId": 1
  }
}
```
### Response  Body
```json 
[
  {
    "id": 1,
    "completionPercentage": 0.0,
    "creationDate": "2023-01-01T00:00:00",
    "updateDate": "2023-01-04T00:00:00",
    "running": 0,
    "queuing": 0,
    "passed": 0,
    "failed": 0,
    "history": [
      {
        "passed": 23,
        "failed": 2,
        "date": "2023-01-01T00:00:00"
      },
      {
        "passed": 23,
        "failed": 2,
        "date": "2023-01-02T00:00:00"
      },
      {
        "passed": 23,
        "failed": 2,
        "date": "2023-01-03T00:00:00"
      }
    ],
    "name": "MyTestSuite",
    "startDate": "2023-01-04T00:00:00",
    "eta": 600, // Seconds
    "status": "ready",
    "tags": [
      "Regression",
      "Smoke"
    ]
  }
]
```

## Get All Test Suites 

### Request Body

```json
{
  "service": "TestCraftService",
  "api": "getAllTestSuite",
  "par": {
    "userId": 1
  }
}
```
### Response  Body
```json 
[
  {
    "id": 1,
    "inputJson": "\"{designs: {}} \"",
    "name": "\"Test222\"",
    "tags": []
  },
  {
    "id": 2,
    "inputJson": "\"\"",
    "name": "\"Test\"",
    "tags": []
  },
  {
    "id": 3,
    "inputJson": "\"{designs: {}} \"",
    "name": "\"Test222\"",
    "tags": []
  }
]
```

## Get Test Suite By ID

### Request Body

```json
{
    "service":"TestCraftService",
    "api":"getTestSuite",
    "par":{
        "id": 5
    }
}
```

### Response  Body
```json 
{
  "id": 1,
  "name": "Test",
  "tags": [
    "smoke",
    "regression"
  ],
  "inputJson": "{designs: {}} "
}
```

## Create Test Suite 

### Request Body

```json
{
  "service": "TestCraftService",
  "api": "postTestSuite",
  "par": {
    "userId": 1,
    "testSuite": {
      "name": "Test",
      "tags": [
        "smoke",
        "regression"
      ],
      "inputJson": "{designs: {}} "
    }
  }
}
```

### Response  Body
```json 
{
  "status": "success",
  "testSuite": {
    "id": 1,
    "name": "Test",
    "tags": [
      "smoke",
      "regression"
    ],
    "inputJson": "{designs: {}} "
  }
}
```


## Edit Test Suite
This is a PUT request. The client should send all the testSuite. 
### Request Body

```json
{
  "service": "TestCraftService",
  "api": "editTestSuite",
  "par": {
    "testSuite": {
      "name": "Test",
      "tags": [
        "smoke",
        "regression"
      ],
      "inputJson": "{designs: {}} "
    }
  }
}
```

### Response  Body
```json 
{
  "status": "success"
}
```

## Delete Test Suite
### Request Body

```json
{
  "service": "TestCraftService",
  "api": "deleteTestSuite",
  "par": {
    "id": 2
  }
}
```

### Response  Body
```json 
{
  "status": "success"
}
```


## Parse Test Suite
### Request Body

```json
{
  "service": "TestCraftService",
  "api": "parseTestSuite",
  "par": {
    "id": 2
  }
}
```

### Response  Body
```json 
[
  {
    "name": "Test Case 1",
    "field": "value"
  },
  {
    "name": "Test Case 2",
    "field": "value"
  }
]
```

## Run Test Suite
### Request Body

```json
{
  "service": "TestCraftService",
  "api": "runTestSuite",
  "par": {
    "id": 5 , 
    "testCases": [ // Ordered
      {
        "name": "Test Case 1",
        "field": "value"
      },
      {
        "name": "Test Case 2",
        "field": "value"
      }
    ]
  }
}
```