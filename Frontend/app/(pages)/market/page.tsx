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

interface Stock {
  company: string;
  type: string;
  price: string;
  change: string;
  profit: string;
}

const Market = () => {
  let emptyStock: Stock = {
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

  const hideActionStockDialog = () => {
    setActionStockDialog(false);
  };

  const actionStock = () => {
    let _stocks;
    if (selectedTypeStock === 1) {
      _stocks = buyStocks.filter((val) => val.company !== stock.company);
      setBuyStocks(_stocks);
    } else {
      _stocks = sellStocks.filter((val) => val.company !== stock.company);
      setBuyStocks(_stocks);
    }

    setActionStockDialog(false);
    setStock(emptyStock);
    let summary = selectedTypeStock === 1 ? "Bought" : "Sold";
    (toast.current as any)?.show({
      severity: "success",
      summary: "Successful",
      detail: "Stock " + summary + " Successfully",
      life: 3000,
    });
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

  useEffect(() => {
    const stocks: Stock[] = [
      {
        company: "Apple",
        type: "Buy",
        price: "$127.13",
        change: "0.23",
        profit: "$1,271.30",
      },
      {
        company: "Microsoft",
        type: "Sell",
        price: "$92.45",
        change: "0.00",
        profit: "$785.60",
      },
      {
        company: "Google",
        type: "Sell",
        price: "$345.78",
        change: "1.12",
        profit: "$2,430.90",
      },
      {
        company: "Amazon",
        type: "Buy",
        price: "$1892.36",
        change: "0.88",
        profit: "$5,678.20",
      },
      {
        company: "Tesla",
        type: "Sell",
        price: "$720.50",
        change: "0.75",
        profit: "$3,456.80",
      },
      {
        company: "Apple",
        type: "Buy",
        price: "$135.80",
        change: "0.42",
        profit: "$987.40",
      },
      {
        company: "Microsoft",
        type: "Buy",
        price: "$102.20",
        change: "-0.91",
        profit: "$1,234.50",
      },
      {
        company: "Google",
        type: "Sell",
        price: "$410.25",
        change: "-1.25",
        profit: "$4,567.90",
      },
      {
        company: "Amazon",
        type: "Buy",
        price: "$2005.60",
        change: "-2.05",
        profit: "$8,765.30",
      },
      {
        company: "Tesla",
        type: "Sell",
        price: "$890.20",
        change: "-0.98%",
        profit: "$6,543.10",
      },
      {
        company: "Apple",
        type: "Buy",
        price: "$115.75",
        change: "0.54",
        profit: "$1,123.60",
      },
      {
        company: "Microsoft",
        type: "Sell",
        price: "$110.30",
        change: "-0.75%",
        profit: "$987.20",
      },
      {
        company: "Google",
        type: "Buy",
        price: "$380.90",
        change: "+1.80%",
        profit: "$3,890.50",
      },
      {
        company: "Amazon",
        type: "Sell",
        price: "$1750.10",
        change: "-1.15%",
        profit: "$5,432.40",
      },
      {
        company: "Tesla",
        type: "Buy",
        price: "$670.40",
        change: "+0.32%",
        profit: "$2,345.60",
      },
      {
        company: "Apple",
        type: "Sell",
        price: "$145.15",
        change: "-0.21%",
        profit: "$1,567.80",
      },
      {
        company: "Microsoft",
        type: "Sell",
        price: "$98.60",
        change: "+1.15%",
        profit: "$1,345.70",
      },
      {
        company: "Google",
        type: "Buy",
        price: "$330.50",
        change: "-0.95%",
        profit: "$4,678.90",
      },
      {
        company: "Amazon",
        type: "Buy",
        price: "$1800.80",
        change: "+1.50%",
        profit: "$7,890.20",
      },
      {
        company: "Tesla",
        type: "Sell",
        price: "$810.30",
        change: "-0.72%",
        profit: "$5,678.90",
      },
      {
        company: "Facebook",
        type: "Buy",
        price: "$300.45",
        change: "+0.85%",
        profit: "$2,345.60",
      },
      {
        company: "Netflix",
        type: "Sell",
        price: "$520.30",
        change: "-0.62%",
        profit: "$3,987.40",
      },
      {
        company: "Twitter",
        type: "Buy",
        price: "$45.20",
        change: "+1.20%",
        profit: "$456.70",
      },
      {
        company: "Snapchat",
        type: "Sell",
        price: "$65.75",
        change: "-0.45%",
        profit: "$789.20",
      },
      {
        company: "Uber",
        type: "Buy",
        price: "$32.90",
        change: "+0.95%",
        profit: "$234.50",
      },
      {
        company: "Lyft",
        type: "Sell",
        price: "$48.60",
        change: "-1.15%",
        profit: "$345.80",
      },
      {
        company: "Airbnb",
        type: "Buy",
        price: "$180.20",
        change: "+2.10%",
        profit: "$1,234.50",
      },
      {
        company: "Zoom",
        type: "Sell",
        price: "$340.90",
        change: "-1.75%",
        profit: "$4,567.80",
      },
      {
        company: "Alibaba",
        type: "Buy",
        price: "$210.60",
        change: "+1.80%",
        profit: "$6,789.20",
      },
      {
        company: "Tencent",
        type: "Sell",
        price: "$380.10",
        change: "-0.98%",
        profit: "$8,765.30",
      },
      {
        company: "Intel",
        type: "Buy",
        price: "$50.75",
        change: "+0.54%",
        profit: "$567.80",
      },
      {
        company: "AMD",
        type: "Sell",
        price: "$95.30",
        change: "-0.75%",
        profit: "$789.20",
      },
      {
        company: "NVIDIA",
        type: "Buy",
        price: "$220.90",
        change: "+1.20%",
        profit: "$2,345.60",
      },
      {
        company: "Samsung",
        type: "Sell",
        price: "$150.20",
        change: "-1.30%",
        profit: "$1,234.50",
      },
      {
        company: "Sony",
        type: "Buy",
        price: "$75.80",
        change: "+0.98%",
        profit: "$987.40",
      },
      {
        company: "LG",
        type: "Sell",
        price: "$40.60",
        change: "-0.85%",
        profit: "$567.80",
      },
      {
        company: "Tesla",
        type: "Buy",
        price: "$650.30",
        change: "+0.65%",
        profit: "$5,678.90",
      },
      {
        company: "SpaceX",
        type: "Sell",
        price: "$800.60",
        change: "-1.50%",
        profit: "$7,890.20",
      },
      {
        company: "Coca-Cola",
        type: "Buy",
        price: "$50.20",
        change: "+0.75%",
        profit: "$456.70",
      },
      {
        company: "PepsiCo",
        type: "Sell",
        price: "$120.90",
        change: "-0.98%",
        profit: "$1,234.50",
      },
    ];
    setBuyStocks(stocks.filter((stock) => stock.type === "Buy"));
    setSellStocks(stocks.filter((stock) => stock.type === "Sell"));
  }, []);

  //   const handleClick = async (event: { preventDefault: () => void }) => {
  //     event.preventDefault();
  //     try {

  //       if (username_or_email === "" || password === "") {
  //         (toast.current as any)?.show({
  //           severity: "error",
  //           summary: "Failed",
  //           detail: "Missing fields",
  //           life: 1500,
  //         });
  //         return;
  //       }

  //       const response = await fetch("http://localhost:8000/login/redirect", {
  //         method: "POST",
  //         headers: {
  //           "Content-Type": "application/json",
  //         },
  //         body: JSON.stringify({
  //           User: {
  //             username_or_email: username_or_email,
  //             password: password,
  //           },
  //         }),
  //       });

  //       const data = await response.json();
  //       console.log(data);
  //       if (data.error) {
  //         (toast.current as any)?.show({
  //           severity: "error",
  //           summary: "Failed",
  //           detail: data.error,
  //           life: 3000,
  //         });
  //         return;
  //       }
  //       (toast.current as any)?.show({
  //         severity: "success",
  //         summary: "Success",
  //         detail: "Login successfully",
  //         life: 1000,
  //       });
  //       setTimeout(() => {
  //         router.push("/");
  //       }, 1000);
  //       console.log(data);
  //     } catch (error) {
  //       console.error("Error sending message:", error as Error);
  //     }
  //   };
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
