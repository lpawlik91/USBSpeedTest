\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{BscUS}[2015/07/08 AGH]

\def\@baseclass{report}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{\@baseclass}}
\PassOptionsToClass{12pt,a4paper,twoside}{\@baseclass}
\ProcessOptions
\LoadClass{\@baseclass}

\RequirePackage{polski}
\RequirePackage{ifthen}

\RequirePackage{geometry}
\geometry{verbose,a4paper,tmargin=2.5cm,bmargin=2.5cm,lmargin=3cm,rmargin=2cm}

\RequirePackage[pdftex,pdfstartview=FitH,unicode]{hyperref}

\RequirePackage{caption}
\captionsetup{font=small,format=plain,labelsep=period,labelfont={bf,it},textfont=it,figurename=Rys.,tablename=Rys.}


\linespread{1.3} % interlinia 1.5


\def\university#1{\def\university{#1}}
\def\@university{Akademia G{\'o}rniczo-Hutnicza w Krakowie}

\def\faculty#1{\gdef\@faculty{#1}}
\def\@faculty{Wydzia{\l} Fizyki i Informatyki Stosowanej}

\def\title#1{\gdef\@title{#1}}
\def\@title{???\ClassError{BscUS}{Brak tytulu pracy}\@ehc}

\def\keywords#1{\gdef\@keywords{#1}}
\def\@keywords{???\ClassError{BscUS}{Brak slow kluczowych}\@ehc}

\def\author#1{\gdef\@author{#1}}
\def\@author{???\ClassError{BscUS}{Brak autora pracy}\@ehc}

\def\promoter#1{\gdef\@promoter{#1}}
\def\@promoter{???\ClassError{BscUS}{Brak opiekuna pracy}\@ehc}

\def\where#1{\gdef\@where{#1}}
\def\@where{Kraków}

\def\when#1{\gdef\@when{#1}}
\def\@when{\number\year}


\newcommand\updatepdfmeta{
	\ifpdf
		\pdfcompresslevel=9
		\hypersetup{pdfauthor={\@author},pdftitle={\@title},pdfkeywords={\@keywords}}
	\fi
} % newcommand: updatepdfmeta

\AtBeginDocument{\updatepdfmeta}


\renewcommand\maketitle{
	\begin{titlepage}

			\includegraphics[height=37.5mm]{./img/agh}\\
				\rule{30mm}{0pt}
			{\large{\@faculty}}\\
			\rule[4pt]{\textwidth}{3pt}\\
			\rule[20pt]{\textwidth}{1pt}\\
			\vskip 18pt
			
			\begin{center}
			{ \bf \Large{Praca magisterska}\par}
				\vskip 50pt
			{\bf \large{ \@author} \par}
				\vskip 8pt
			{\small{ kierunek studi{\'o}w:}} {\bf\small {Informatyka Stosowana}}
				\vskip 50pt	
			{\bf {\setstretch{1.0}\LARGE{\@title}\par
}}
				\vskip 65pt
			{\large {Promotor: \bf \@promoter}\par}
				\vskip 80pt
			{\bf  {\@where, \@when}}

			\end{center}		
	\end{titlepage}
} % renewcommand: maketitle


\newcommand\makestatement{


\begin{center}
\begin{flushleft}
 O{\'s}wiadczam, {\'s}wiadomy odpowiedzialno{\'s}ci karnej za po{\'s}wiadczenie nieprawdy, {\.z}e niniejsz{\k{a}} prac{\k{e}} dyplomow{\k{a}} wykona{\l}em osobi{\'s}cie i samodzielnie i nie korzysta{\l}em ze {\'z}r{\'o}de{\l} innych ni{\.z} wymienione w pracy.
\end{flushleft}

	\vskip 36pt


\begin{flushright}
\begin{tabular}{lr}
&................................................................. \\
& {(czytelny podpis)}\\
\end{tabular}
\end{flushright}
\end{center}

} % newcommand: makestatement


\makeatletter


% naglowki numerowane z kropka, bez prefixu 'Rozdzia�'
\renewcommand \thechapter {\@Roman\c@chapter}

\renewcommand{\@makechapterhead}[1]{%
%	\vspace*{10pt}%
	{\raggedright\normalfont\bfseries\LARGE\thechapter.\ #1
	 \par\nobreak\vspace{15pt}}
}

% naglowki bez numerow, kompatybilnosc stylu
\renewcommand{\@makeschapterhead}[1]{%
%	\vspace*{10pt}%
	{\raggedright\normalfont\bfseries\LARGE #1
	 \par\nobreak\vspace{15pt}}
}

% kropki po numerach sekcji
\renewcommand\@seccntformat[1]{\csname the#1\endcsname.\ }

% kropki po numrach w spisie tresci
\renewcommand\numberline[1]{#1.\hskip0.7em}

\makeatother
