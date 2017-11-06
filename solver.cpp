#include "solver.h"

using namespace std;

ofstream out("work.tex");
ofstream maple("maple.txt");
ofstream outHtml("output.html");

Vector* e;

int n = 2;
const int Vector::n = 2;

double epsilon = 0.01;

int a, b, p, q;

int zadanie;

Vector Min, Max;

int iter;

//------------------------------------------------------------------------------

double func1(double x_1, double x_2) {
    return a * x_1 * x_1 + 2 * x_1 * x_2  + b * x_2 * x_2 - 2 * x_1 - 3 * x_2;
}

double func2(double x, double y) {
    return sqrt(pow(x-y, 2) + pow(a * x * x - y - b, 2));
}

double func3(double x_1, double x_2) {
    return pow(x_1 - p, 2) + pow(x_2 - q, 2);
}

double func4(double x_1, double x_2) {
    return a * pow(x_2 - x_1 * x_1, 2) + pow(b - x_1, 2);
}

typedef double (*Function)(double, double);

Function functions[4] = {&func1, &func2, &func3, &func4};

double f(const Vector& v) {
    return functions[zadanie-1](v[1], v[2]);
}

//--------------------------------------------------------

void Solver::makeIteration(const Vector& x) {
    if(html)
        outHtml <<"<tr><td>"<< ++iter << "</td><td>" << x[1] << "</td><td>"
           << x[2]<< "</td>"
           <<"<td>"<< f(x) << "</td></tr>" << endl;
    if(latex)
        out << ++iter << "&" << x << "& " << f(x) << "\\\\ \\hline" << endl;
    maple << ",[" << x[1] << "," << x[2] << "]";
    for(int i = 1; i <= n; i++) {
        Min[i] = min(Min[i], x[i]);
        Max[i] = max(Max[i], x[i]);
    }
}

double abs(const Vector& v) {
    double sum = 0;
    for(int i = 1; i <= Vector::n; i++) {
        sum += v[i]*v[i];
    }
    return sqrt(sum);
}

Vector poisk(Vector x, double Delta) {
    for(int i = 1; i <= n; i++) {
        if(f (x + Delta * e[i]) < f(x))
            x += Delta * e[i];
        else if(f (x - Delta * e[i]) < f(x))
            x -= Delta * e[i];
    }
    return x;
}

Vector Solver::Hooke_Jeeves(Vector x) {
    double Delta = 1;
    double lambda = 2;
    while(Delta > epsilon) {
        while(1) {
            Vector y = poisk(x, Delta);
            if(y == x)
                break;
            while (f(y) < f(x)) {
                Vector obr = y - x;
                x = y;
                makeIteration(x);
                y = x + (lambda - 1) * obr;
                y = poisk(y, Delta);
            }
        }
        Delta /= 2;
    }
    return x;
}

Vector Solver::Coordinate_Descent(Vector x) {
    for(;;) {
        Vector x_0 = x;
        for(int i = 1; i <= n; i++) {
            double Delta = 1;
            while(Delta > epsilon) {
                while(f(x + Delta * e[i]) < f(x))
                    x += Delta * e[i];
                while (f(x - Delta * e[i]) < f(x))
                    x -= Delta * e[i];
                Delta /= 2;
            }
            makeIteration(x);
        }
        if(abs(x - x_0) < 0.1 * epsilon)
            break;
    }
    return x;
}

Vector Solver::Rosenbrock(Vector x) {
    double alpha = 3;
    double beta = -0.5;
    Vector * s = new Vector[n] - 1;
    double* Lambda = new double[n] - 1;
    double* lambda = new double[n] - 1;
    Vector * A = new Vector[n] - 1;
    Vector * B = new Vector[n] - 1;
    int* b = new int[n]-1;
    for(int i = 1; i <= n; i++)
        s[i] = e[i];
    for(;;) {
        int sum = 2 * n;
        memset(Lambda+1, 0, n * sizeof(double));
        for(int i = 1; i <= n; i++) {
            lambda[i] = 1;
            b[i] = 2;
        }
        int J = 100;
        for(int j = 0; j < J; j++) {
            int i = j%n + 1;
            if(f(x + lambda[i] * s[i]) <= f(x)) {
                x += lambda[i] * s[i];
                Lambda[i] += lambda[i];
                lambda[i] *= alpha;
                if(b[i] == 2)
                {
                    b[i] = 1;
                    sum--;
                }
            } else {
                lambda[i] *= beta;
                if(b[i] == 1)
                {
                    b[i] = 0;
                    sum--;
                }
                if(!sum)
                    break;
            }
        }
        makeIteration(x);
        A[1] = Lambda[1] * s[1];
        for(int i = 2; i<= n; i++) {
            A[1] += Lambda[i] * s[i];
        }
        if(abs(A[1]) < epsilon)
            break;
        for(int i = 2; i <= n; i++)
            A[i] = A[i-1] - Lambda[i-1] * s[i-1];
        Vector * s_k = new Vector[n]-1;
        for(int i = 1; i <= n; i++)
            s_k[i] = s[i];
        s[1] = A[1] / abs(A[1]);
        for(int i = 2; i <= n; i++) {
            B[i] = A[i];
            for(int j = 1; j < i; j++)
                B[i] -= (A[i] * s[j]) * s[j];
            if(!abs(B[i]))
                s[i] = s_k[i-1];
            else
                s[i] = B[i]/ abs(B[i]);
        }
        delete [] ++s_k;
    }
    delete [] ++lambda;
    delete [] ++Lambda;
    delete [] ++s;
    delete [] ++b;
    delete [] ++B;
    delete [] ++A;
    return x;
}

Vector Solver::Powell (Vector x) {
    Vector * s = new Vector[n] - 1;
    for(int i = 1; i <= n; i++)
        s[i] = e[i];
    for(;;) {
        Vector x_0 = x;
        int m = 1;
        double Delta_k = 0;
        for(int i = 1; i <= n; i++) {
            Vector y = x;
            double Delta = 1;
            while(Delta > epsilon) {
                while(f(y + Delta * s[i]) < f(y))
                    y += Delta * s[i];
                while (f(y - Delta * s[i]) < f(y))
                    y -= Delta * s[i];
                Delta /= 2;
            }
            if(abs(y - x) > Delta_k) {
                m = i;
                Delta_k = abs(y - x) ;
            }
            x = y;
        }
        double f_1 = f(x_0);
        double f_2 = f(x);
        double f_3 = f(2*x - x_0);
        if(f_3 <= f_1 || (f_1 - 2 * f_2 + f_3) * pow(f_1 - f_2 - Delta_k, 2) >= 0.5 *
                Delta_k * (f_1 - f_2 - Delta_k) * pow(f_1 - f_3, 2)) {
            if(f_3 <= f_2)
                x = 2 * x - x_0;
        } else {
            for(int i = m; i <= n-1; i++)
                s[i] = s[i+1];
            s[n] = x - x_0;
            double Delta = 1;
            while(Delta > epsilon) {
                while(f(x + Delta * s[n]) < f(x))
                    x += Delta * s[n];
                while (f(x - Delta * s[n]) < f(x))
                    x -= Delta * s[n];
                Delta /= 2;
            }
        }
        makeIteration(x);
        if(abs(x-x_0) < 0.1* epsilon)
            break;
    }
    delete[] ++s;
    return x;
}

Vector Solver::Nelder_Mead(Vector X) {
    double alpha = 1, beta = 0.5, gamma = 2;
    Vector * x = new Vector[n+1]-1;
    x[1] = X;
    for(int i = 1; i <= n; i++)
        x[i+1] = X + e[i];
    double * f_ = new double[n+1]-1;
    for(int i = 1; i <= n + 1; i++)
        f_[i] = f(x[i]);
    for(;;) {
        int h = 1, g = 2, l = 1;
        for(int i = 2; i <= n+1; i++) {
            if(f_[i] >= f_[h]) {
                g = h;
                h = i;
            }
            else if (f_[i] > f_[g])
                g = i;
            else if(f_[i] < f_[l])
                l = i;
        }
        Vector x_0;
        for(int i = 1; i <= n+1; i++) {
            if(i!= h)
                x_0 += x[i];
        }
        x_0 /= n;
        double f_0 = f(x_0);
        Vector x_r = (1 + alpha) * x_0 - alpha * x[h];
        double f_r = f(x_r);
        if(f_r <= f_[l]) {
            Vector x_epsilon = gamma * x_r + (1 - gamma) * x_0;
            double f_epsilon = f(x_epsilon);
            if(f_epsilon < f(x[l])) {
                x[h] = x_epsilon;
                f_[h] = f_epsilon;
            } else {
                x[h] = x_r;
                f_[h] = f_r;
            }
        }
        else if(f_r <= f_[g]) {
            x[h] = x_r;
            f_[h] = f_r;
        }
        else {
            if(f_r <= f_[h]) {
                x[h] = x_r;
                f_[h] = f_r;
            }
            Vector x_c = x_0 + beta * (x[h] - x_0);
            double f_c = f(x_c);
            if(f_c <= f_[h]) {
                x[h] = x_c;
                f_[h] = f_c;
            } else {
                for(int i = 1 ; i <= n+1; i++) {
                    x[i] = 0.5 * (x[l] + x[i]);
                    f_[i] = f(x[i]);
                }
            }
        }
        double Mf = f_[1];
        double Df = f_[1] * f_[1];
        for(int i = 2; i <= n+1; i++) {
            Mf += f_[i];
            Df += f_[i] * f_[i];
        }
        Mf /= n + 1;
        Df /= n + 1;
        Df -= Mf * Mf;
        makeIteration(x[l]);
        if(sqrt(Df) < epsilon) {
            Vector result =  x[l];
            delete [] ++x;
            delete [] ++f_;
            return result;
        }
    }
}

void Solver::solve(int variant) {
    out.open("work.tex");
    maple.open("maple.txt");
    outHtml.open("output.html");
    setlocale(LC_ALL, "russian");
    qDebug()<<variant<<" " << html << " " << latex<<endl;
    if(html)
        outHtml<<"<html><meta charset='UTF-8'>"
              <<"<head><style> table, th, td {"
              <<"border: 1px solid black;"
              <<"}"
                <<"* {font-size:18px;}"
                <<"</style></head>"
              <<endl;
    e = new Vector[n];
    e--;
    for(int i = 1; i <= n; i++)
        e[i] = Vector::ort(i);
    emit sendMsg("Начало вычислений");
    emit progress(0.02);

    Vector begin;

    ifstream is("variants.txt");
    string methods[5] = {"Хука-Дживса", "покоординатного спуска", "Розенброка", "Пауэлла" ,"Нелдера-Мида"};
    int table = 1;
    for(zadanie = 1; zadanie <= 4; zadanie++) {
        stringstream result;
        stringstream resultHtml;

        resultHtml<< "<table style='border:1; width:100%'>";
        result << "\\begin{table}[H]" << endl;

        resultHtml << "<tr><td>Методы</td><td>Число итераций</td><td>X_1opt</td><td>X_2opt</td>"
                      "<td>F(X_1opt,X_2opt)</td></tr>";
        result << "\\hspace{0cm}\\rightline{Таблица " << table + 5 << ". Результаты}" << endl
        <<"\\begin{center}" << endl
        << "\\begin{tabular}{|p{3.1cm}|p{1.4cm}|p{2.1cm}|p{2.1cm}|p{3cm}|}" << endl
        << "\\hline"
        << "Методы&Число итераций&$X_{1opt}$&$X_{2opt}$&$F(X_{1opt},X_{2opt})$\\\\" << endl
        << "\\hline" << endl;
        if(html)
            outHtml << "Задача " << zadanie << "." << endl << endl;
        if(latex)
            out << "Задача " << zadanie << "." << endl << endl;
        string s;
        if(zadanie == 1) {
            for(int j = 0; j < 2 + 2 * variant%10; j++)
                getline(is, s);
        } else {
            for(int j = 0; j < 24; j++)
                getline(is, s);
        }
        if(zadanie == 3) {
            is >> p >> p >> q;
            if(variant > 10)
                is >> p >> p >> q;
        } else {
            is >> a >> a >> b;
            if(variant > 10)
                is >> a >> a >> b;
        }
        maple << "with(plots):"<<endl;
        maple << "F1:=(x,y)-> " << a << "* x* x + 2 * x * y  + " <<b << " * y * y - 2 * x - 3 * y;"<<endl;
        maple << "F2:=(x,y)-> sqrt((x-y)^2 + ("<<a<<" * x * x - y - "<<b<<")^2);"<<endl;
        maple << "F3:=(x,y)-> (x - ("<<p<<") )^2 + (y - "<<q<<")^2;"<<endl;
        maple << "F4:=(x,y)-> "<<a<<" * (y - x * x)^2 + ("<<b<<" - x)^2;"<<endl;

        for(int i = 0; i < 5; i++) {
            Min = begin;
            Max = begin;
            if(html)
                outHtml << "Результаты расчёта методом " << methods[i] << ":" << endl << endl;
            if(latex)
                out << "Результаты расчёта методом " << methods[i] << ":" << endl << endl;

            if(html)
                outHtml << "<table style='border:1; width:100%'>"
                    << "<tr><td># итерации</td><td>X1</td><td>X2</td><td>F(X1,X2)</td></tr>";
            if(latex)
                out << "\\begin{table}[H]" << endl
                    << "\\hspace{0cm}\\rightline{Таблица " << table++ << ". Метод " << methods[i] <<  "}" << endl
                    << "\\begin{center}" << endl
                    << "\\begin{tabular}{|p{3cm}|p{4.2cm}|p{4.2cm}|p{4.2cm}|}" << endl
                    << "\\hline"
                    << "&\\multicolumn{3}{p{12.6cm}|}{\\textbf{Полученные значения}}\\\\" << endl
                    << "\\cline{2-4}" << endl
                    << "\\raisebox{1.5ex}[0cm][0cm]{\\textbf{№ итерации }}&\\vspace{0.1cm}" << endl
                    << "{\\textbf{X1}} &\\vspace{0.1cm} {\\textbf{X2}}&\\vspace{0.1cm}{\\textbf{F(X1,X2)}} \\\\" << endl
                    << "\\hline" << endl;
            maple << "plot1:=plot([[0,0]";
            iter = 0;
            Vector x_opt;
            switch(i) {
                case 0:
                    x_opt = Hooke_Jeeves(begin);
                    break;
                case 1:
                    x_opt = Coordinate_Descent(begin);
                    break;
                case 2:
                    x_opt = Rosenbrock(begin);
                    break;
                case 3:
                    x_opt = Powell(begin);
                    break;
                case 4:
                    x_opt = Nelder_Mead(begin);
                    break;
            }
            resultHtml << "<tr><td>" << methods[i] << " </td><td> " << iter << "</td><td>" << x_opt[1] << "</td><td> "<<x_opt[2] <<"</td><td>"<< f(x_opt) << "</td></tr>";
            result << methods[i] << " & " << iter << "&" << x_opt << "& " << f(x_opt) << "\\\\ \\hline";
            if(html)
                outHtml<<"</table><br/><br/>";
            if(latex)
                out << "\\end{tabular}" << endl
                    << "\\end{center}" << endl
                    << "\\end{table} " << endl << endl;
            Vector radius;
            radius[1] = max(x_opt[1] - Min[1], Max[1] - x_opt[1]);
            radius[2] = max(x_opt[2] - Min[2], Max[2] - x_opt[2]);
            Min = x_opt - radius;
            Max = x_opt + radius;
            if(latex) {
                out << "Теперь изобразим траекторию поиска методом " << methods [i] <<" (рисунок \\ref{img"<<zadanie<<i<<"})." << endl << endl;
                out<<"\\begin{figure}[H]"<<endl
                  <<"\\begin{center}"<<endl
                 <<"\\includegraphics[width=10cm]{output"<<zadanie<<i<<"}"<<endl
                <<"\\caption{\Задача " << zadanie << ". Метод " << methods[i] << "}"<<endl
                <<"\\label{img"<<zadanie<<i<<"}"<<endl
                 <<"\\end{center}"<<endl
                <<"\\end{figure}"<<endl;
            }
            maple << "], x = " << Min[1] << ".." << Max[1] << ", y = " <<  Min[2] << ".." << Max[2] <<", color=red):" << endl;
            maple << "plot2 := contourplot(F" << zadanie << "(x, y), x =  " << Min[1] << ".." << Max[1] << ", y = " <<  Min[2] << ".." << Max[2] <<", color = blue):" << endl;
            maple << "plotsetup(ps, plotoutput=\"./Images/output"<<zadanie<<i<<".eps\", plotoptions=`portrait,noborder,color`);"<<endl;
            maple << "display(plot1, plot2);" << endl << endl;

        }
        result << "\\end{tabular}" << endl
               << "\\end{center}" << endl
               << "\\end{table}" << endl;
        resultHtml << "</table>";
        if(latex)
            out << "Итоговые результаты: " << endl << result.str();
        if(html)
            outHtml << "Итоговые результаты : " << endl << resultHtml.str();
        progress(zadanie * 10.0 / 100.0 + 0.12);
    }
    if(html)
        outHtml << "<br/><br/><h1>Код для визуализации в Maple. </h1>"
            <<"<object data='./maple.txt' type='text/plain' style='border: 1px solid black;height: 600px; width: 100%; background: #d3d3d3;'>"
            <<"<a href='./maple.txt'>No Support?</a>"
            <<"</object>"
            <<"<html>";
    emit sendMsg("Сохранение отчета html");
    progress(0.65);

    outHtml.close();
    out.close();
    maple.close();
    delete [] ++e;
    if(latex) {
        system("rm latex_log maple_log ");
        emit sendMsg("Лог работы расположен в файлах latex_log и maple_log соответственно");
        emit sendMsg("maple начал построение графиков");
        system("maple ./maple.txt > ./maple_log");
        emit sendMsg("maple закончил построение графиков");
        emit progress(0.70);
        emit sendMsg("latex начал сборку отчета");
        system("pdflatex  -synctex=1 -interaction=nonstopmode report.tex > /dev/null");
        system("pdflatex  -synctex=1 -interaction=nonstopmode report.tex > ./latex_log");
        emit sendMsg("latex закончил сборку отчета");
        emit progress(0.90);
        emit sendMsg("удаление временных файлов");

        system("rm *.aux");
        system("rm *.out");
        system("rm *.log");
        system("rm *.xml");
        system("rm *.bcf");


        system("evince report.pdf");

    }
    emit progress(1);
    emit sendMsg("Завершено!");



}
