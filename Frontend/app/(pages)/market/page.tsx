"use client";
import { GBtn, Navbar } from "../../components";
import { useEffect, useRef, useState } from "react";
import { Toast } from "primereact/toast";
import "primereact/resources/themes/lara-light-blue/theme.css";
import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";
import "./scss/market.css";
import { DataTable } from "primereact/datatable";
import { Column } from "primereact/column";
import { Button } from "primereact/button";
import React from "react";
import { Dialog } from "primereact/dialog";
import { useRouter } from "next/navigation";
import { getSessionToken } from "../../utils/cookie";
import { ProgressSpinner } from "primereact/progressspinner";
import ReconnectingWebSocket from "reconnecting-websocket";

interface Stock {
  id: number;
  company: string;
  type: string;
  price: number;
  change: number;
  profit: number;
  initial_price: number;
  available_stocks: number;
}

const Market = () => {
  let emptyStock: Stock = {
    id: 0,
    company: "",
    type: "",
    price: 0,
    change: 0,
    profit: 0,
    initial_price: 0,
    available_stocks: 0,
  };
  const toast = useRef(null);
  const [buyStocks, setBuyStocks] = useState<Stock[]>([]);
  const [sellStocks, setSellStocks] = useState<Stock[]>([]);
  const [selectedTypeStock, setSelectedTypeStock] = useState(1);
  const [stock, setStock] = useState<Stock>(emptyStock);
  const [actionStockDialog, setActionStockDialog] = useState<boolean>(false);
  const [nextUpdate, setNextUpdate] = useState(10);
  const [wallet, setWallet] = useState(0);
  const [quantity, setQuantity] = useState(0);
  const clearEveryThing = () => {
    setBuyStocks([]);
    setSellStocks([]);
    setStock(emptyStock);
    setActionStockDialog(false);
    setNextUpdate(10);
  };

  const router = useRouter();
  const sessionToken = getSessionToken();
  if (!sessionToken) {
    router.push("/login");
    return;
  }

  const hideActionStockDialog = () => {
    setActionStockDialog(false);
  };
  const fetchWallet = async () => {
    try {
      const response = await fetch(
        `http://localhost:8001/api/getUser?token=${getSessionToken()}`,
        {
          method: "GET",
          headers: {
            "Content-Type": "application/json",
          },
        }
      );

      const data = await response.json();
      if (data.error) {
        console.log(data.error);
        return;
      }
      setWallet(data["User"].wallet);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  const actionStock = async () => {
    let _stocks;

    if (selectedTypeStock === 1) {
      _stocks = buyStocks.filter((val) => val.company !== stock.company);
      try {
        const response = await fetch("http://localhost:8001/api/market/buy", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            token: getSessionToken(),
            stock: stock.id,
            quantity: quantity,
          }),
        });

        const data = await response.json();
        if (data.error) {
          (toast.current as any)?.show({
            severity: "error",
            summary: "Failed",
            detail: data.error,
            life: 3000,
          });
          return;
        }
        fetchWallet();
        setActionStockDialog(false);
        setStock(emptyStock);
        setBuyStocks(_stocks);
        setQuantity(0);

        let summary = selectedTypeStock === 1 ? "Bought" : "Sold";
        (toast.current as any)?.show({
          severity: "success",
          summary: "Successful",
          detail: "Stock " + summary + " Successfully",
          life: 3000,
        });
      } catch (error) {
        console.error("Error sending message:", error as Error);
      }
    } else {
      _stocks = sellStocks.filter((val) => val.company !== stock.company);
      try {
        const response = await fetch("http://localhost:8001/api/market/sell", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            token: getSessionToken(),
            stock: stock.id,
            quantity: quantity,
          }),
        });

        const data = await response.json();
        if (data.error) {
          (toast.current as any)?.show({
            severity: "error",
            summary: "Failed",
            detail: data.error,
            life: 3000,
          });
          return;
        }
        setActionStockDialog(false);
        setStock(emptyStock);
        setQuantity(0);

        let summary = selectedTypeStock === 1 ? "Bought" : "Sold";
        setBuyStocks(_stocks);
        (toast.current as any)?.show({
          severity: "success",
          summary: "Successful",
          detail: "Stock " + summary + " Successfully",
          life: 3000,
        });
      } catch (error) {
        console.error("Error sending message:", error as Error);
      }
    }
  };

  const confirmActionStock = (rowStock: Stock) => {
    if (quantity > (rowStock.available_stocks ?? 0)) {
      (toast.current as any)?.show({
        severity: "info",
        summary: "Info",
        detail: "Quantity must be less than available stocks",
        life: 1000,
      });
      return;
    }
    if (quantity > 0) {
      setStock(rowStock);
      setActionStockDialog(true);
    } else {
      (toast.current as any)?.show({
        severity: "info",
        summary: "Info",
        detail: "Quantity must be greater than 0",
        life: 1000,
      });
    }
  };
  const deleteTestDialogFooter = (
    <React.Fragment>
      <Button
        label="No"
        icon="pi pi-times"
        outlined
        style={{ width: "90px", height: "40px" }}
        onClick={hideActionStockDialog}
      />
      <Button
        label="Yes"
        icon="pi pi-check"
        severity="danger"
        style={{ width: "90px", height: "40px" }}
        onClick={actionStock}
      />
    </React.Fragment>
  );

  const fetchData = async () => {
    if (selectedTypeStock === 1) {
      try {
        clearEveryThing();

        const url = `http://localhost:8001/api/market/getAllStocks?token=${getSessionToken()}`;

        const response = await fetch(url, {
          method: "GET",
          headers: {
            "Content-Type": "application/json",
          },
        });

        const data = await response.json();
        if (data.error) {
          console.log(data.error);
          return;
        }
        let _buyStocks: Stock[] = data["stocks"].map((stockData: any) => ({
          id: stockData.id,
          company: stockData.company,
          type: "Buy",
          price: stockData.current_price,
          change: "0",
          profit: "0",
          initial_price: stockData.initial_price,
          available_stocks: stockData.available_stocks,
        }));
        setBuyStocks(_buyStocks);
        let _sellStocks: Stock[] = [];
        for (let i = 0; i < _buyStocks?.length; i++) {
          if (
            _buyStocks[i].company ==
            sellStocks?.find((stock) => stock.company == _buyStocks[i].company)
              ?.company
          ) {
            _sellStocks.push(_buyStocks[i]);
          }
        }
        console.log("SELL STOCKS", _sellStocks);
        setSellStocks(_sellStocks);
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    }
  };

  const fetchStockCart = async () => {
    const url = `http://localhost:8001/api/market/getStockCart?token=${getSessionToken()}`;

    const response = await fetch(url, {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    });

    const data = await response.json();
    if (data.error) {
      console.log(data.error);
      return;
    }
    // setSellStocks(data["stocks"]);
    let _sellStocks: Stock[] = [];
    for (let key in data["stocks"]) {
      let curStock: Stock = data["stocks"][key];
      if (
        curStock.company ==
        buyStocks?.find((stock) => stock.company == curStock.company)?.company
      ) {
        const index = buyStocks.findIndex(
          (stock) => stock.company == curStock.company
        );
        curStock.price = buyStocks[index].price;
        curStock.change = buyStocks[index].change;
        curStock.profit = buyStocks[index].profit;
        curStock.initial_price = buyStocks[index].initial_price;
        _sellStocks.push(curStock);
      }
    }
    setSellStocks(_sellStocks);
    console.log("MY STOCKS", _sellStocks);
    console.log("GET QUERY", data);
  };
  useEffect(() => {
    setNextUpdate(10);
    fetchStockCart();
    fetchWallet();
  }, [buyStocks]);

  useEffect(() => {
    const socket = new WebSocket("ws://127.0.0.1:8083");

    socket.addEventListener("open", (event) => {
      console.log("WebSocket connection opened:", event);
      fetchData();
      fetchWallet();
      fetchStockCart();
    });

    socket.addEventListener("message", (event) => {
      console.log("WebSocket message received:", event.data);
      const data = JSON.parse(event.data);
      let _buyStocks: Stock[] = [];
      for (let key in data) {
        console.log(data[key]);
        let _stock: Stock = {
          id: data[key].id,
          company: data[key].company,
          type: "Buy",
          price: data[key].current_price,
          change: data[key].change,
          profit: data[key].profit,
          initial_price: data[key].initial_price,
          available_stocks: data[key].available_stocks,
        };
        _buyStocks.push(_stock);
      }
      setBuyStocks(_buyStocks);

      let _sellStocks: Stock[] = [];
      for (let i = 0; i < _buyStocks?.length; i++) {
        if (
          _buyStocks[i].company ==
          sellStocks?.find((stock) => stock.company == _buyStocks[i].company)
            ?.company
        ) {
          _sellStocks.push(_buyStocks[i]);
        }
      }
      console.log("SELL STOCKS", _sellStocks);
      setSellStocks(_sellStocks);
    });

    socket.addEventListener("close", (event) => {
      console.log("WebSocket connection closed:", event);
      fetchData();
      fetchWallet();
      fetchStockCart();
    });

    // Countdown mechanism
    const countdownInterval = setInterval(() => {
      setNextUpdate((prevValue) => (prevValue === 0 ? 10 : prevValue - 1));
    }, 1000);

    return () => {
      // Close the WebSocket connection when the component is unmounted
      socket.close();
      clearInterval(countdownInterval);
    };
  }, []);
  // useEffect(() => {
  //   // Check the condition before setting up intervals
  //   if (selectedTypeStock === 1) {
  //     // Fetch data initially
  //     fetchData();

  //   // If selectedType is not 1, do nothing on mount and return an empty cleanup function
  //   return () => {};
  // }, [selectedTypeStock]);

  const actionView = (rowData: any) => {
    return (
      <div
        className={
          selectedTypeStock === 1
            ? "market-action-btn-buy"
            : "market-action-btn-sell"
        }
      >
        <GBtn
          btnText={selectedTypeStock === 1 ? "Buy" : "Sell"}
          clickEvent={() => {
            confirmActionStock(rowData);
            {
              selectedTypeStock === 1;
            }
          }}
          icnSrc={
            selectedTypeStock === 1 ? "/SVG/dollar.svg" : "/Images/sell.png"
          }
        />
      </div>
    );
  };
  const changeBodyTemplate = (rowData: { change: string }) => {
    const changeValue = parseFloat(rowData.change);

    if (changeValue > 0) {
      return <span style={{ color: "green" }}>+{changeValue.toFixed(2)}%</span>;
    } else if (changeValue < 0) {
      return <span style={{ color: "red" }}>{changeValue.toFixed(2)}%</span>;
    } else {
      // Change is zero
      return <span>{changeValue.toFixed(2)}%</span>;
    }
  };
  const changeBodyInitPriceTemplate = (rowData: { initial_price: string }) => {
    const changeValue = parseFloat(rowData.initial_price);
    return <span>${changeValue.toFixed(2)}</span>;
  };
  const changeBodyPriceTemplate = (rowData: { price: string }) => {
    const changeValue = parseFloat(rowData.price);
    return <span>${changeValue.toFixed(2)}</span>;
  };
  const changeBodyProfitTemplate = (rowData: { profit: string }) => {
    const changeValue = parseFloat(rowData.profit);
    if (changeValue > 0) {
      return <span>${changeValue.toFixed(2)}</span>;
    } else if (changeValue == 0) {
      return <span>$0.00</span>;
    } else {
      let _changeValue = changeValue * -1;
      return <span>-${_changeValue.toFixed(2)}</span>;
    }
  };

  return (
    <>
      <Navbar idx={2} />
      <div className="market-container">
        <Toast ref={toast} />
        <div className="m-c-section1">
          <h1>Stock Market</h1>
          <h3>Don't miss a beat with global real-time updates.</h3>
          <div className="m-c-s1-btns">
            {selectedTypeStock === 1 ? (
              <div className="clicked">
                <GBtn
                  btnText="Buy"
                  clickEvent={() => {
                    setSelectedTypeStock(1);
                  }}
                />
              </div>
            ) : (
              <GBtn
                btnText="Buy"
                clickEvent={() => {
                  setSelectedTypeStock(1);
                }}
              />
            )}

            {selectedTypeStock === 2 ? (
              <div className="clicked">
                <GBtn
                  btnText="Sell"
                  clickEvent={() => {
                    setSelectedTypeStock(2);
                  }}
                />
              </div>
            ) : (
              <GBtn
                btnText="Sell"
                clickEvent={() => {
                  setSelectedTypeStock(2);
                }}
              />
            )}
          </div>
          <span className="market-balance">
            Current Balance: ${wallet.toFixed(2)}
          </span>
          <div className="market-timer">
            <span>Refresh after: {nextUpdate}s</span>
          </div>
        </div>
        <div className="m-c-section2">
          <div style={{ width: "100%" }}>
            <DataTable
              value={selectedTypeStock === 1 ? buyStocks : sellStocks}
              stripedRows
              paginator
              removableSort
              sortMode="multiple"
              rows={10}
              size={"normal"}
              rowsPerPageOptions={[10, 50, 100, 200]}
              emptyMessage={
                <div
                  style={{
                    display: "flex",
                    justifyContent: "center",
                    alignItems: "center",
                  }}
                >
                  <ProgressSpinner style={{ width: "50px" }} />
                </div>
              }
            >
              <Column field="company" sortable header="Company"></Column>
              <Column
                field="available_stocks"
                sortable
                header="Available Stocks"
              />
              <Column
                field="initial_price"
                sortable
                header="Initial Price"
                body={changeBodyInitPriceTemplate}
              />
              <Column
                field="price"
                sortable
                header="Price "
                body={changeBodyPriceTemplate}
              ></Column>
              <Column
                field="change"
                sortable
                body={changeBodyTemplate}
                header="Current Change %"
              ></Column>
              <Column
                field="profit"
                sortable
                header="Profit"
                body={changeBodyProfitTemplate}
              ></Column>
              <Column
                header={
                  <div className="counting-container">
                    <span
                      onClick={() => {
                        setQuantity(Math.max(quantity - 1, 0));
                      }}
                    >
                      -
                    </span>
                    <h3>{quantity}</h3>
                    <span
                      onClick={() => {
                        setQuantity(quantity + 1);
                      }}
                    >
                      +
                    </span>
                  </div>
                }
                body={actionView}
                style={{ width: "120px" }}
              ></Column>
            </DataTable>
            <Dialog
              visible={actionStockDialog}
              style={{ width: "32rem" }}
              breakpoints={{ "960px": "75vw", "641px": "90vw" }}
              header="Confirm"
              modal
              footer={deleteTestDialogFooter}
              onHide={hideActionStockDialog}
            >
              <div className="confirmation-content">
                <i
                  className="pi pi-exclamation-triangle mr-3"
                  style={{ fontSize: "2rem" }}
                />
                {stock && (
                  <span>
                    Are you sure you want to{" "}
                    {selectedTypeStock == 1 ? "buy" : "sell"}{" "}
                    <b>{stock.company + " stock"}</b>?
                  </span>
                )}
              </div>
            </Dialog>
          </div>
        </div>
      </div>
    </>
  );
};

export default Market;
