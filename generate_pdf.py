import os
import re

folders = [
    "Template",
    "Data_Structures",
    "Graphs",
    "Math",
    "Number_Theory",
    "Combinatorics",
    "Geometry",
    "String",
    "DP",
    "Misc",
    "Helpers"
]

def escape_latex(text):
    return text.replace('_', r'\_').replace('&', r'\&').replace('#', r'\#').replace('%', r'\%')

def format_title(text):
    return escape_latex(text.replace('_', ' '))

def clean_and_write_code(filepath, f):
    try:
        with open(filepath, 'r', encoding='utf-8') as code_file:
            content = code_file.read()
            
        content = content.encode('ascii', 'ignore').decode('ascii')
        
        f.write("\\begin{lstlisting}[language=C++]\n")
        f.write(content + "\n")
        f.write("\\end{lstlisting}\n")
    except Exception as e:
        pass

with open("main.tex", "w", encoding="utf-8") as f:
    f.write(r"""\documentclass[10pt,landscape,a4paper,twocolumn]{article}
\usepackage[utf8]{inputenc}
\usepackage[english]{babel}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{geometry}
\usepackage{fancyhdr}
\usepackage{hyperref}
\usepackage{tocloft}

\geometry{top=1cm, bottom=1.5cm, left=1cm, right=1cm, includehead, headheight=15pt, headsep=0.3cm}

\definecolor{codegreen}{rgb}{0,0.6,0}
\definecolor{codegray}{rgb}{0.5,0.5,0.5}
\definecolor{codepurple}{rgb}{0.58,0,0.82}
\definecolor{backcolour}{rgb}{0.96,0.96,0.96}
\definecolor{linkcolor}{rgb}{0,0.3,0.6}
\definecolor{keywordcolor}{rgb}{0.8,0.1,0.4}

\hypersetup{
    colorlinks=true,
    linkcolor=linkcolor,
    filecolor=magenta,      
    urlcolor=blue,
    pdftitle={ECPC Reference Document},
    pdfauthor={Tarek Accepted 3shan Zh2t}
}

\lstdefinestyle{mystyle}{
    backgroundcolor=\color{backcolour},   
    commentstyle=\itshape\color{codegreen},
    keywordstyle=\bfseries\color{keywordcolor},
    numberstyle=\tiny\color{codegray},
    stringstyle=\color{codepurple},
    basicstyle=\ttfamily\footnotesize,
    breakatwhitespace=false,         
    breaklines=true,                 
    captionpos=b,                    
    keepspaces=true,                 
    numbers=left,                    
    numbersep=5pt,                  
    showspaces=false,                
    showstringspaces=false,
    showtabs=false,                  
    tabsize=4,
    frame=single,
    rulecolor=\color{codegray}
}
\lstset{style=mystyle}

\pagestyle{fancy}
\fancyhf{}
\fancyhead[L]{\textbf{Tarek Accepted 3shan Zh2t} - Tanta University}
\fancyhead[C]{ECPC Team Reference Document}
\fancyhead[R]{\textbf{Page \thepage}}
\renewcommand{\headrulewidth}{0.4pt}

\fancypagestyle{plain}{
    \fancyhf{}
    \fancyhead[L]{\textbf{Tarek Accepted 3shan Zh2t} - Tanta University}
    \fancyhead[C]{ECPC Team Reference Document}
    \fancyhead[R]{\textbf{Page \thepage}}
    \renewcommand{\headrulewidth}{0.4pt}
}

\setlength{\cftsecnumwidth}{2.5em}
\setlength{\cftsubsecnumwidth}{3em}
\setcounter{tocdepth}{3}

\begin{document}
\tableofcontents
\newpage
""")

    for folder in folders:
        if not os.path.exists(folder):
            continue
            
        f.write(f"\\section{{{format_title(folder)}}}\n")
        
        items = sorted(os.listdir(folder))
        for item in items:
            item_path = os.path.join(folder, item)
            
            if os.path.isfile(item_path):
                if item.endswith((".cpp", ".hpp", ".h")):
                    name = os.path.splitext(item)[0]
                    f.write(f"\\subsection{{{format_title(name)}}}\n")
                    clean_and_write_code(item_path, f)
            
            elif os.path.isdir(item_path):
                f.write(f"\\subsection{{{format_title(item)}}}\n")
                
                sub_items = sorted(os.listdir(item_path))
                for sub_item in sub_items:
                    sub_item_path = os.path.join(item_path, sub_item)
                    
                    if os.path.isfile(sub_item_path) and sub_item.endswith((".cpp", ".hpp", ".h")):
                        name = os.path.splitext(sub_item)[0]
                        f.write(f"\\subsubsection{{{format_title(name)}}}\n")
                        clean_and_write_code(sub_item_path, f)

    f.write(r"\end{document}" + "\n")