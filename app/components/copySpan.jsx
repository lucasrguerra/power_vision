"use strict";

export default function CopySpan(text) {
    return (
        <span
            className="text-blue-700 font-semibold"
            onClick={(event) => {
                navigator.clipboard.writeText(text);
                event.target.style.textDecoration = "none";
                event.target.style.cursor = "default";
                event.target.innerHTML = "Copiado!";
                setTimeout(() => {
                    event.target.innerHTML = text;
                }, 1000);
            }}
            onMouseEnter={(event) => {
                event.target.style.cursor = "pointer";
                event.target.style.textDecoration = "underline";
            }}
            onMouseLeave={(event) => {
                event.target.style.cursor = "default";
                event.target.style.textDecoration = "none";
            }}
        >{text}</span>
    );
};