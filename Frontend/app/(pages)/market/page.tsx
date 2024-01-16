"use client";
import { GBtn, Navbar } from "../../components";
import { use, useEffect, useRef, useState } from "react";
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

interface Stock {
  id: number;
  company: string;
  type: string;
  price: string;
  change: string;
  profit: string;
}

const Market = () => {
  let emptyStock: Stock = {
    id: 0,
    company: "",
    type: "",
    price: "",
    change: "",
    profit: "",
  };
  const toast = useRef(null);
  const [buyStocks, setBuyStocks] = useState<Stock[]>([]);
  const [sellStocks, setSellStocks] = useState<Stock[]>([]);
  const [selectedTypeStock, setSelectedTypeStock] = useState(1);
  const [stock, setStock] = useState<Stock>(emptyStock);
  const [actionStockDialog, setActionStockDialog] = useState<boolean>(false);
  const [nextUpdate, setNextUpdate] = useState(20);
  const clearEveryThing = () => {
    setBuyStocks([]);
    setSellStocks([]);
    setSelectedTypeStock(1);
    setStock(emptyStock);
    setActionStockDialog(false);
    setNextUpdate(20);
  };

  const hideActionStockDialog = () => {
    setActionStockDialog(false);
  };

  const actionStock = async () => {
    let _stocks;
    if (selectedTypeStock === 1) {
      _stocks = buyStocks.filter((val) => val.company !== stock.company);
      try {
        const response = await fetch("http://localhost:8000/market/buy", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            User: 1,
            Stock: stock.id,
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
        setBuyStocks(_stocks);

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
        const response = await fetch("http://localhost:8000/market/sell", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            User: 1,
            Stock: stock.id,
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
    setStock(rowStock);
    setActionStockDialog(true);
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
    try {
      clearEveryThing();
      const response = await fetch(
        "http://localhost:8000/market/getAllStocks",
        {
          method: "GET",
          headers: {
            "Content-Type": "application/json",
          },
        }
      );
      const data = await response.json();

      const shuffledStocks = shuffleArray(data.stocks);

      setBuyStocks(
        shuffledStocks.filter((stock: { type: string }) => stock.type === "Buy")
      );
      setSellStocks(
        shuffledStocks.filter(
          (stock: { type: string }) => stock.type === "Sell"
        )
      );
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };
  const shuffleArray = (array: any) => {
    for (let i = array.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [array[i], array[j]] = [array[j], array[i]];
    }
    return array;
  };

  useEffect(() => {
    // Fetch data initially
    fetchData();

    // Set up interval to fetch data every 10 seconds
    const fetchDataIntervalId = setInterval(fetchData, 20000);

    // Set up interval to decrement nextUpdate every second
    const countdownIntervalId = setInterval(() => {
      setNextUpdate((prevNextUpdate) => prevNextUpdate - 1);
    }, 1000);

    // Clear intervals on component unmount
    return () => {
      clearInterval(fetchDataIntervalId);
      clearInterval(countdownIntervalId);
    };
  }, []);

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

  return (
    <>
      <Navbar idx={2} />
      <div className="market-container">
        <Toast ref={toast} position="bottom-right" />
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
          <span className="market-balance">Current Balance: $217</span>
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
            >
              <Column field="company" sortable header="Company"></Column>
              <Column field="price" sortable header="Price "></Column>
              <Column
                field="change"
                sortable
                body={changeBodyTemplate}
                header="Change %"
              ></Column>
              <Column field="profit" sortable header="Profit"></Column>
              <Column body={actionView} style={{ width: "120px" }}></Column>
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
