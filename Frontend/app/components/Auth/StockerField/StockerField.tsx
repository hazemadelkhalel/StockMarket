"use client";
import { StockerInputProps } from "../../../shared/types";
import "./scss/stockerfield.css";

const StockerField = (props: StockerInputProps) => {
  return (
    <div className="stocker-input">
      <span className="gi-text">{props.giText}</span>
      <input type={props.giType} placeholder={props.giPlaceholder} />
    </div>
  );
};

export default StockerField;
