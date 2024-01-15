// button props
export interface btnProps {
  btnText: string;
  icnSrc?: string;
  clickEvent: any;
}

// Groofy Input props
export interface StockerInputProps {
  giText: string;
  giPlaceholder: string;
  giType: string;
  onInputChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
}
