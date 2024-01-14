"use client";
import Link from "next/link";
import { GBtn, StockerField } from "../../components";
import "./scss/signup.css";

const SignUp = () => {
  return (
    <div className="align">
      <div className="signup-div">
        <div className="features">
          <div className="ft-title">
            Stock<span>Market</span>
          </div>
          <div className="ft-container">
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/stockIcon.svg" alt="BadgeIcon" />
              </div>
              <div className="ftb-info">
                <h4>Stay Ahead with Live Market Updates</h4>
                <p>
                  Access real-time stock quotes, market indices, and dynamic
                  charts, empowering users with up-to-the-minute data for
                  informed investment decisions.
                </p>
              </div>
            </div>
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/analysisIcon.svg" alt="BadgeIcon" />
              </div>
              <div className="ftb-info">
                <h4>In-depth Analysis for Informed Choices</h4>
                <p>
                  Uncover market trends and insights through a rich news
                  section, providing expert commentary and analysis on financial
                  developments, ensuring users are well-equipped with the latest
                  information.
                </p>
              </div>
            </div>
            <div className="ft-box">
              <div className="ftb-icn">
                <img src="/SVG/toolsIcon.svg" alt="ShieldIcon" />
              </div>
              <div className="ftb-info">
                <h4>Effortless Portfolio Tracking and Management</h4>
                <p>
                  Seamlessly track and manage your investments with intuitive
                  portfolio tools, allowing users to monitor asset allocation,
                  analyze performance, and make strategic adjustments for a
                  well-balanced investment portfolio.
                </p>
              </div>
            </div>
          </div>
        </div>
        <div className="auth">
          <div className="auth-title">
            Sign up as a <span>Stocker</span>
          </div>
          <form className="auth-form">
            <StockerField
              giText="Username"
              giPlaceholder="Enter your username"
              giType="text"
            />
            <StockerField
              giText="Email"
              giPlaceholder="Enter your email"
              giType="email"
            />
            <StockerField
              giText="Password"
              giPlaceholder="Enter your password"
              giType="password"
            />
            <StockerField
              giText="Confirm Password"
              giPlaceholder="Confirm your password"
              giType="password"
            />
            <div className="f-sbmt">
              <GBtn btnText="Create new account" clickEvent={() => {}} />
              <span className="alrg">
                Already have an account?<Link href="/login">Login</Link>
              </span>
            </div>
          </form>
        </div>
      </div>
    </div>
  );
};

export default SignUp;
