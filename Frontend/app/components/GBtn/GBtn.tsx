"use client";
import { btnProps } from "../../shared/types";
import "./scss/gbtn.css";

const GBtn = (props: btnProps) => {
  return (
    <button className="groofybtn" onClick={props.clickEvent}>
      {props.icnSrc && (
        <img className="btn-icn" src={`${props.icnSrc}`} alt="Run" />
      )}
      <span className="btn-text">{props.btnText}</span>
    </button>
  );
};

export default GBtn;
