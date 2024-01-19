# API Documentation

## Get All Stocks

### Request Parameter

- API: `/api/market/getAllStocks`

- Method: `GET`

- Example: `/api/market/getAllStocks?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE`

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE"
}
```

### Response Body

```json
{
  "message": "Retrieve all stocks successfully!",
  "status": "success",
  "stocks": [
    {
      "available_stocks": 996,
      "change": 0.0,
      "company": "Apple",
      "current_price": 150.83395385742188,
      "id": 1,
      "initial_price": 127.12999725341797,
      "profit": 0.0
    },
    {
      "available_stocks": 800,
      "change": 0.0,
      "company": "Microsoft",
      "current_price": 175.71336364746094,
      "id": 2,
      "initial_price": 92.44999694824219,
      "profit": 0.0
    },
    {
      "available_stocks": 1200,
      "change": 0.0,
      "company": "Google",
      "current_price": 345.7799987792969,
      "id": 3,
      "initial_price": 345.7799987792969,
      "profit": 0.0
    }
  ]
}
```

## Get Stock Cart for User

- API: `/api/market/getStockCart`

- Method: `GET`

- Example: `/api/market/getStockCart?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE`

### Request Parameter

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE"
}
```

### Response Body

```json
{
  "message": "Retrieve all stocks successfully!",
  "status": "success",
  "stocks": [
    {
      "available_stocks": 1,
      "change": 0.0,
      "company": "Snapchat",
      "current_price": 2.4517617225646973,
      "id": 9,
      "initial_price": 65.75,
      "profit": 0.0
    },
    {
      "available_stocks": 1,
      "change": 0.0,
      "company": "Google",
      "current_price": 345.7799987792969,
      "id": 3,
      "initial_price": 345.7799987792969,
      "profit": 0.0
    }
  ]
}
```

## Update Info for User

- API: `/api/profile/update`

- Method: `POST`

### Request Body

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJ6ZXpvMTIxMiJ9.ZcsM0dLSrfXEPaDQIJ4eGS5cPAXPkhOMi9yVACcXY6w",
  "user": {
    "aboutme": "Hello World!",
    "card_number": "123456789",
    "created_at": "2024-01-18T15:41:05",
    "email": "zeze_ghost@gmail.com",
    "facebook_profile": "",
    "first_name": "",
    "id": 3,
    "instagram_profile": "",
    "last_name": "",
    "password": "123456",
    "phone": "",
    "username": "zezo1212",
    "wallet": 1200.0,
    "website": ""
  }
}
```

### Response Body

```json
{
  "message": "Update user successfully!",
  "status": "success",
  "user": {
    "aboutme": "Hello World!",
    "card_number": "123456789",
    "created_at": "2024-01-18T15:41:05",
    "email": "zeze_ghost@gmail.com",
    "facebook_profile": "",
    "first_name": "",
    "id": 3,
    "instagram_profile": "",
    "last_name": "",
    "password": "123456",
    "phone": "",
    "username": "zezo1212",
    "wallet": 1200.0,
    "website": ""
  }
}
```

## Note:

- ("username", "id", created_at", "wallet", "password") are fixed parameter and they doesn't change even if you send them in the body of the json, otherwise they will update.

- You don't have to send all parameter (It will be an empty string).

Acceptable Example:

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJ6ZXpvMTIxMiJ9.ZcsM0dLSrfXEPaDQIJ4eGS5cPAXPkhOMi9yVACcXY6w",
  "user": {}
}
```

Response:

```json
{
  "message": "Update user successfully!",
  "status": "success",
  "user": {
    "aboutme": "",
    "card_number": "",
    "created_at": "2024-01-18T15:41:05",
    "email": "",
    "facebook_profile": "",
    "first_name": "",
    "id": 3,
    "instagram_profile": "",
    "last_name": "",
    "password": "123456",
    "phone": "",
    "username": "zezo1212",
    "wallet": 1200.0,
    "website": ""
  }
}
```

## Get All Transaction for a user

- API: `/api/profile/getAllTransactions`

- Method: `GET`

- Example: `/api/market/getAllTransactions?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE`

### Request Parameter

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.SQhSKK8HzGJlcmyI-oZ5ZvnTqGULcPB8biyjMlPUgLk"
}
```

### Response Body

```json
{
  "message": "Retrieve all transactions successfully!",
  "status": "success",
  "transactions": [
    {
      "balance": 1200.0,
      "company": "Apple",
      "id": 1,
      "price": 127.21986389160156,
      "quantity": 1,
      "transaction_date": "2024-01-18T04:54:13",
      "type": "Buy",
      "userID": 1
    },
    {
      "balance": 1072.7801513671875,
      "company": "Facebook",
      "id": 2,
      "price": 300.2166748046875,
      "quantity": 1,
      "transaction_date": "2024-01-18T04:54:31",
      "type": "Buy",
      "userID": 1
    },
    {
      "balance": 772.5634765625,
      "company": "Uber",
      "id": 3,
      "price": 26.423919677734375,
      "quantity": 1,
      "transaction_date": "2024-01-18T04:54:37",
      "type": "Buy",
      "userID": 1
    }
  ]
}
```

## Get User

- API: `/api/getUser`

- Method: `GET`

- Example: `/api/getUser?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.K3gv-CcLCvusTTRy0-TaeCbg4rhj419JyzNp93EXOTE`

### Request Parameter

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.AuXZbiC0MZQpee3l8nnhjpPm4XByjOM7K9vJiKAFmK0"
}
```

### Response Body

```json
{
  "User": {
    "aboutme": "I want to work at siemens :)",
    "card_number": "14141241412",
    "created_at": "2024-01-18T04:54:00",
    "email": "hazemadelkhalel@gmail.com",
    "facebook_profile": "www.facebook.com/hazemadelkhalel",
    "first_name": "Hazem",
    "id": 1,
    "instagram_profile": "www.instagram.com/hazemadelkhalel",
    "last_name": "Adel",
    "password": "123456",
    "phone": "01116104321",
    "username": "hazemadelkhalel",
    "wallet": 91.91152954101563,
    "website": ""
  },
  "message": "Retrieve user successfully!",
  "status": "success"
}
```

## Signup (Create new user)

- API: `/api/signup/create`

- Method: `POST`

### Request Body

```json
{
  "user": {
    "username": "newUser2",
    "email": "newUser2@gmail.com",
    "password": "123456"
  }
}
```

### Response Body

```json
{
  "message": "User created successfully!",
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJuZXdVc2VyMiJ9.pMnOoPGQCnr5ocaqLuOZGQzGfXQiGXplBSSEiFUtUH0"
}
```

## Login user

- API: `/api/login/redirect`

- Method: `POST`

### Request Body

```json
{
  "user": {
    "username_or_email": "hazemadelkhalel",
    "password": "123456"
  }
}
```

### Response Body

```json
{
  "message": "Login successfully!",
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.SQhSKK8HzGJlcmyI-oZ5ZvnTqGULcPB8biyjMlPUgLk"
}
```

## Buy a new stock

- API: `/api/market/buy`

- Method: `POST`

### Request Body

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJ6ZXpvMTIxMiJ9.ZcsM0dLSrfXEPaDQIJ4eGS5cPAXPkhOMi9yVACcXY6w",
  "stock": 1,
  "quantity": 1
}
```

### Response Body

```json
{
  "success": "Stock bought successfully",
  "Transaction": {
    "balance": 763.9171142578125,
    "company": "Microsoft",
    "id": 17,
    "price": 175.71336364746094,
    "quantity": 1,
    "transaction_date": "2024-01-19T04:26:03",
    "type": "Buy",
    "userID": 3
  }
}
```

## Sell a stock

- API: `/api/market/sell`

- Method: `POST`

```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCIsInVzZXIiOiJoYXplbWFkZWxraGFsZWwifQ.VasY9AtgAtswuILjXG8TKBCcLRnC-mpYbDe-pKPFuBA",
  "stock": 1,
  "quantity": 1
}
```

### Response Body

```json
{
  "success": "Stock sold successfully"
}
```
