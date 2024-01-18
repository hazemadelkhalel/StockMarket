"use client";

import Link from "next/link";
import "./scss/navbar.css";
import { useState } from "react";
import { Divider } from "primereact/divider";
import { useRouter } from "next/navigation";
import { removeSessionToken } from "../../utils/cookie";

const Navbar = (props: { idx: number; username: string }) => {
  const [profileActive, setProfileActive] = useState(false);
  const [isHovered, setIsHovered] = useState(false);
  const router = useRouter();

  const handleMouseEnter = () => {
    setIsHovered(true);
  };

  const handleMouseLeave = () => {
    setIsHovered(false);
  };

  const handleClickLogout = async (event: { preventDefault: () => void }) => {
    removeSessionToken();
    router.push("/login");
  };
  return (
    <nav className="nav-container">
      <Link className="nav-link" href="/">
        <div className="nav-logo">
          <img src="Images/Logo.png" alt="logo" />
          <h1>Stock Market</h1>
        </div>
      </Link>
      <ul className="nav-links">
        <li>
          <Link
            className={`${props.idx == 0 ? "nav-link active" : "nav-link "}`}
            href="/"
          >
            Home
          </Link>
        </li>
        <li>
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
            Markets
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
      </ul>
      <div
        className="nav-user"
        onClick={() => {
          setProfileActive((state) => !state);
        }}
      >
        <span>
          {props.username && props.username.length > 0
            ? props.username[0].toUpperCase()
            : ""}
        </span>
      </div>
      <div className={`profile-area ${profileActive}`}>
        <Link href="/profile">
          <div className="pa-info">
            <span className="home-user-img">
              {props.username && props.username.length > 0
                ? props.username[0].toUpperCase()
                : ""}
            </span>
            <span>{props.username}</span>
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
        <div className="profile-area-space"></div>
        <Link href="/login" onClick={handleClickLogout}>
          <div
            className={`logout ${isHovered ? "active-logout" : ""}`}
            onMouseEnter={handleMouseEnter}
            onMouseLeave={handleMouseLeave}
          >
            <img
              className="logoutImage"
              src={
                isHovered ? "/SVG/white-logout.svg" : "/SVG/black-logout.svg"
              }
              alt="Logout"
            />
            <span>Logout</span>
          </div>
        </Link>
      </div>
    </nav>
  );
};

export default Navbar;
