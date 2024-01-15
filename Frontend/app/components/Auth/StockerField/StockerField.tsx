"use client";
import { useState } from "react";
import { StockerInputProps } from "../../../shared/types";
import "./scss/stockerfield.css";

const StockerField = (props: StockerInputProps) => {
  const [fieldValue, setFieldValue] = useState("");

  const handleInputChange = (event: { target: { value: any } }) => {
    const value = event.target.value;
    setFieldValue(value);
    props.onInputChange(value);
  };
  return (
    <div className="stocker-input">
      <span className="gi-text">{props.giText}</span>
      <input
        type={props.giType}
        placeholder={props.giPlaceholder}
        value={fieldValue}
        onChange={handleInputChange}
      />
    </div>
  );
};

export default StockerField;
