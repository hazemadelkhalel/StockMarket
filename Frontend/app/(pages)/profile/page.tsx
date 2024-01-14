"use client";
import Link from "next/link";
import { GBtn, Navbar, StockerField } from "../../components";
import "./scss/profile.css";
import { Divider } from "primereact/divider";
import { Button } from "primereact/button";
import "primereact/resources/themes/lara-light-blue/theme.css";

import "primereact/resources/primereact.min.css";
import "primeicons/primeicons.css";
import "primeflex/primeflex.css";
import { useState } from "react";

const Profile = () => {
  const [selectedTap, setSelectedTap] = useState(1);
  return (
    <>
      <Navbar idx={0} />
      <div className="profile-container">
        <section className="profile-section-1">
          <div className="profile-pc">
            <img src="/Images/Me.jpg" />
          </div>
          <div className="profile-info">
            <div className="p-i-data">
              <h1>hazemadelkhalel</h1>
              <div className="p-i-creation">
                <img src="/SVG/calendar.svg" />
                <span>Joined 6 months ago.</span>
              </div>
            </div>
            <div className="p-i-container-progress">
              <div className="p-i-progress">
                <h3>14</h3>
                <span>Buy</span>
              </div>
              <div className="p-i-progress">
                <h3>5</h3>
                <span>Sell</span>
              </div>
              <div className="p-i-progress">
                <h3>19</h3>
                <span>Transactions</span>
              </div>
              <div className="p-i-progress">
                <h3>3</h3>
                <span>Stocks</span>
              </div>
            </div>
          </div>
        </section>
        <Divider />
        <section className="profile-section-2">
          <div className="p-s-2-header">
            <h2
              className={`${
                selectedTap == 1 ? "nav-link active" : "nav-link "
              }`}
              onClick={() => {
                setSelectedTap(1);
              }}
            >
              My Stocks
            </h2>
            <h2
              className={`${
                selectedTap == 2 ? "nav-link active" : "nav-link "
              }`}
              onClick={() => {
                setSelectedTap(2);
              }}
            >
              History
            </h2>
            <h2
              className={`${
                selectedTap == 3 ? "nav-link active" : "nav-link "
              }`}
              onClick={() => {
                setSelectedTap(3);
              }}
            >
              Settings
            </h2>
          </div>
        </section>
        <Divider />
      </div>
    </>
  );
};

export default Profile;
