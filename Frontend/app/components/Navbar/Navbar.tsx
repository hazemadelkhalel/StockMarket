"use client";

import Link from "next/link";
import "./scss/navbar.css";
import { useState } from "react";

const Navbar = (props: { idx: number }) => {
  const [profileActive, setProfileActive] = useState(false);
  return (
    <nav className="nav-container">
      <Link className="nav-link" href="/">
        <div className="nav-logo">
          <img src="Images/Logo.png" alt="logo" />
          <h1>Stock Market</h1>
        </div>
      </Link>
      <ul className="nav-links">
        <li className="">
          <Link
            className={`${props.idx == 1 ? "nav-link active" : "nav-link "}`}
            href="/companies"
          >
            Top Companies
          </Link>
        </li>
        <li>
          <Link
            className={`${props.idx == 2 ? "nav-link active" : "nav-link "}`}
            href="/market"
          >
            Market
          </Link>
        </li>
        <li>
          <Link
            className={`${props.idx == 3 ? "nav-link active" : "nav-link "}`}
            href="/news"
          >
            News
          </Link>
        </li>
        <li>
          <Link
            className={`${props.idx == 4 ? "nav-link active" : "nav-link "}`}
            href="/about"
          >
            About
          </Link>
        </li>
      </ul>
      <div
        className="nav-user"
        onClick={() => {
          setProfileActive((state) => !state);
        }}
      >
        <img src="SVG/userIcon.svg" alt="user" />
      </div>
      <div className={`profile-area ${profileActive}`}>
        <Link href="/login">
          <div className="pa-info">
            <img src="/SVG/userIconColored.svg" alt="ProfilePicture" />
            <span>Sign In</span>
          </div>
        </Link>
        <Link href="/help">
          <div className="logout">
            <span>Help Center</span>
          </div>
        </Link>
        <Link href="/news">
          <div className="logout">
            <span>What's new</span>
          </div>
        </Link>
      </div>
    </nav>
  );
};

export default Navbar;
