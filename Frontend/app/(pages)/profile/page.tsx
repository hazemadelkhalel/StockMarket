"use client";
import Link from "next/link";
import { GBtn, Navbar, StockerField } from "../../components";
import "./scss/profile.css";
import { Divider } from "primereact/divider";
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
            <Link href="/profile/#mystocks" scroll={false}>
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
            </Link>
            <Link href="/profile/#history" scroll={false}>
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
            </Link>
            <Link href="/profile/#settings" scroll={false}>
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
            </Link>
          </div>
        </section>
        <Divider />
        <div className="profile-section-3">
          {selectedTap == 1 ? <div>STOCK</div> : null}
          {selectedTap == 2 ? <div>history</div> : null}
          {selectedTap == 3 ? (
            <div className="p-s-3-settings-container">
              <div className="settings-left">
                <h1>Public Info</h1>
                <p>
                  This information will be publicly displayed and visible for
                  all users.
                </p>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Username</span>
                    <input type="text" placeholder="Username" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">About Me</span>
                    <textarea placeholder="" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Website</span>
                    <input type="text" placeholder="Website" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Facebook profile</span>
                    <input type="text" placeholder="Username" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Instagram profile</span>
                    <input type="text" placeholder="Username" />
                  </div>
                </div>
                <div className="settings-btn">
                  <GBtn btnText="Save changes" clickEvent={() => {}} />
                </div>
              </div>
              <div className="settings-right">
                <h1>Private Details</h1>
                <p>This information will not be publicly displayed.</p>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Email</span>
                    <input type="text" placeholder="Email" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">First Name</span>
                    <input type="text" placeholder="First Name" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Last Name</span>
                    <input type="text" placeholder="Last Name" />
                  </div>
                </div>
                <div className="settings-item">
                  <div className="stocker-item-input">
                    <span className="gi-item-text">Phone</span>
                    <input type="text" placeholder="Phone" />
                  </div>
                </div>
                <div className="settings-btn">
                  <GBtn btnText="Save changes" clickEvent={() => {}} />
                </div>
              </div>
            </div>
          ) : null}
        </div>
      </div>
    </>
  );
};

export default Profile;
