#include "stdafx.h"
#include <algorithm>
#include <map>
#include "logicCalc.h"

typedef std::map<CString, int>::iterator mapIt;

//���������������ȼ�
const int PRI_NEGATIVE = 1;
const int PRI_CONJUNCTION = 2;
const int PRI_DISJUNCTION = 3;
const int PRI_INCLUSION = 4;
const int PRI_EQUIVALENCE = 5;
const int PRI_LEFT_PARENTHESIS = 7;
const int PRI_RIGHT_PARENTHESIS = 6;
const int PRI_ERROR = -1;

const int TYPE_OP = true;
const int TYPE_VAR = false;

const CString STRING_TRUE = CString("1");
const CString STRING_FALSE = CString("0");

int inputContent[MAXCONTENT]; //�洢����õĹ�ʽ
bool inputType[MAXCONTENT];
int inputCnt;
bool varValue[MAXCONTENT]; //�洢�����ĸ�ֵ
int varNumberCnt;

//���ȼ����ǡ���ȡ����ȡ���̺����ȼۡ������š����������ȼ�����Ϊ1��2��3��4��5��0��6
inline int GetPrioity(const TCHAR &c)
{
    if (c == SIGN_NEGATIVE)
    {
        return PRI_NEGATIVE;
    }
    else if (c == SIGN_CONJUNCTION)
    {
        return PRI_CONJUNCTION;
    }
    else if (c == SIGN_DISJUNCTION)
    {
        return PRI_DISJUNCTION;
    }
    else if (c == SIGN_INCLUSION)
    {
        return PRI_INCLUSION;
    }
    else if (c == SIGN_EQUIVALENCE)
    {
        return PRI_EQUIVALENCE;
    }
    else if (c == SIGN_LEFT_PARENTHESIS)
    {
        return PRI_LEFT_PARENTHESIS;
    }
    else if (c == SIGN_RIGHT_PARENTHESIS)
    {
        return PRI_RIGHT_PARENTHESIS;
    }
    return PRI_ERROR;
}

//��������ȼ��Ƚ�
inline bool CmpPriority(const int &opa, const int &opb)
{
    return (opa <= opb);
}

//�ж��Ƿ�Ŀ��������������ȼ���
inline bool IsUnaryOperatorPri(const int &pri)
{
    return (pri == PRI_NEGATIVE);
}

//�ж��Ƿ�˫Ŀ��������������ȼ���
inline bool IsBinocularOperatorPri(const int &pri)
{
    return ((pri == PRI_CONJUNCTION) || (pri == PRI_DISJUNCTION) || (pri == PRI_INCLUSION) || (pri == PRI_EQUIVALENCE));
}

//�ж��Ƿ��������
inline bool IsOperator(const TCHAR &c)
{
    return ((c == SIGN_NEGATIVE) || (c == SIGN_CONJUNCTION) || 
        (c == SIGN_DISJUNCTION) || (c == SIGN_INCLUSION) || (c == SIGN_EQUIVALENCE) || 
        (c == SIGN_LEFT_PARENTHESIS) || (c == SIGN_RIGHT_PARENTHESIS));
}

//�������ȽϺ���
inline bool VarNameCmp(const mapIt &ita, const mapIt &itb)
{
    if (ita->first == STRING_TRUE)
    {
        return false;
    }
    if (itb->first == STRING_TRUE)
    {
        return true;
    }
    if (ita->first == STRING_FALSE)
    {
        return false;
    }
    if (itb->first == STRING_FALSE)
    {
        return true;
    }
    return ita->first < itb->first;
}

//��������ı��ʽ�������㣩
inline bool DealInput(const CString &sInput, int &varNumberCnt, CString variable[])
{
    std::map<CString, int> varNumber; //��������������ŵ�ӳ��
    varNumber.clear();
    varNumberCnt = 0;
    mapIt varIt[MAXCONTENT]; //���ڱ���������ĸ�������

    inputCnt = 0;
    int i = 0;
    while (i < sInput.GetLength())
    {
        if (IsOperator((sInput[i])))
        {
            inputType[inputCnt] = TYPE_OP;
            inputContent[inputCnt++] = GetPrioity((sInput[i++]));
        }
        else
        {
            inputType[inputCnt] = TYPE_VAR;
            int j = i++;
            while ((i < (int)sInput.GetLength()) && (!IsOperator(sInput[i])))
            {
                i++;
            }
            CString varName = sInput.Mid(j, i - j);
            if (varNumber.find(varName) != varNumber.end())
            {
                inputContent[inputCnt++] = varNumber[varName];
            }
            else
            {
                inputContent[inputCnt++] = varNumber[varName] = ++varNumberCnt;
                varIt[varNumberCnt - 1] = varNumber.find(varName);
            }
        }
    }

    //���������������±��
    std::sort(varIt, varIt + varNumberCnt, VarNameCmp);
    int tzb[MAXCONTENT + 1];
    for (int i = 0; i < varNumberCnt; i++)
    {
        variable[i] = varIt[i]->first;
        tzb[varIt[i]->second] = i;
    }
    for (int i = 0; i < inputCnt; i++)
    {
        if (inputType[i] == TYPE_VAR)
        {
            inputContent[i] = tzb[inputContent[i]];
        }
    }
    
    //�Ա�����Ϊ"0","1"�ģ�����У���ȷ���丳ֵ
    if ((varNumberCnt > 0) && (variable[varNumberCnt - 1] == STRING_TRUE))
    {
        varValue[--varNumberCnt] = true;
        if ((varNumberCnt > 0) && (variable[varNumberCnt - 1] == STRING_FALSE))
        {
            varValue[--varNumberCnt] = false;
        }
    }
    else if ((varNumberCnt > 0) && (variable[varNumberCnt - 1] == STRING_FALSE))
    {
        varValue[--varNumberCnt] = false;
    }
    
    //�ж������Ƿ�ƥ��
    int flag = 0;
    for (int i = 0; i < inputCnt; i++)
    {
        if (inputType[i] == TYPE_OP)
        {
            if (inputContent[i] == PRI_LEFT_PARENTHESIS)
            {
                flag++;
            }
            else if (inputContent[i] == PRI_RIGHT_PARENTHESIS)
            {
                if (flag-- == 0)
                {
                    return false;
                }
            }
        }
    }
    if (flag > 0)
    {
        return false;
    }
    
    //�жϱ��ʽ�Ƿ�Ϸ�
    for (int i = 1; i < inputCnt; i++)
    {
        if ((inputType[i - 1] == TYPE_VAR) || (inputContent[i - 1] == PRI_RIGHT_PARENTHESIS))
        {
            if (!(IsBinocularOperatorPri(inputContent[i]) || (inputContent[i] == PRI_RIGHT_PARENTHESIS)))
            {
                return false;
            }
        }
        else// if (IsUnaryOperatorPri(inputContent[i - 1]) || IsBinocularOperatorPri(inputContent[i - 1]) || (inputContent[i - 1] == PRI_LEFT_PARENTHESIS))
        {
            if (!((inputType[i] == TYPE_VAR) || IsUnaryOperatorPri(inputContent[i]) || (inputContent[i] == PRI_LEFT_PARENTHESIS)))
            {
                return false;
            }
        }
    }
    
    return true;
}

//����ջ�����������һ�μ���
inline void CalcOnce(int op[], int &opCnt, bool value[], int &valueCnt)
{
    int &calcOp = op[--opCnt];
    if (calcOp == PRI_NEGATIVE)
    {
        value[valueCnt - 1] = !value[valueCnt - 1];
    }
    else if (calcOp == PRI_CONJUNCTION)
    {
        value[valueCnt - 2] = value[valueCnt - 2] && value[valueCnt - 1];
        valueCnt--;
    }
    else if (calcOp == PRI_DISJUNCTION)
    {
        value[valueCnt - 2] = value[valueCnt - 2] || value[valueCnt - 1];
        valueCnt--;
    }
    else if (calcOp == PRI_INCLUSION)
    {
        value[valueCnt - 2] = (!value[valueCnt - 2]) || value[valueCnt - 1];
        valueCnt--;
    }
    else if (calcOp == PRI_EQUIVALENCE)
    {
        value[valueCnt - 2] = !(value[valueCnt - 2] ^ value[valueCnt - 1]);
        valueCnt--;
    }
}

//����һ�ָ�ֵ��������ʽ��ֵ
inline bool CalcExpression(void)
{
    int op[MAXCONTENT]; //�����ջ
    int opCnt = 0;
    bool value[MAXCONTENT]; //����ֵջ
    int valueCnt = 0;

    for (int i = 0; i < inputCnt; i++)
    {
        if (inputType[i] == TYPE_VAR)
        {
            value[valueCnt++] = varValue[inputContent[i]];
        }
        else// if (inputType[i] == TYPE_OP)
        {
            if (inputContent[i] == PRI_RIGHT_PARENTHESIS)
            {
                while (op[opCnt - 1] != PRI_LEFT_PARENTHESIS)
                {
                    CalcOnce(op, opCnt, value, valueCnt);
                }
                opCnt--;
            }
            else if (inputContent[i] == PRI_LEFT_PARENTHESIS)
            {
                op[opCnt++] = inputContent[i];
            }
            else if (inputContent[i] == PRI_NEGATIVE)
            {
                op[opCnt++] = inputContent[i];
            }
            else
            {
                while (CmpPriority(op[opCnt - 1], inputContent[i]))
                {
                    CalcOnce(op, opCnt, value, valueCnt);
                }
                op[opCnt++] = inputContent[i];
            }
        }
    }
    return value[0];
}

//ö�����и�ֵ�����������CalcExpression����ÿ�ָ�ֵ�Ľ��
void Dfs(const int deep, const int index, CString &sTruthTable, const int &varNumberCnt)
{
    if (deep == varNumberCnt)
    {
        sTruthTable += (TCHAR)('0' + (int)CalcExpression());
        return;
    }
    varValue[deep] = false;
    Dfs(deep + 1, index << 1, sTruthTable, varNumberCnt);
    varValue[deep] = true;
    Dfs(deep + 1, index << 1 | 1, sTruthTable, varNumberCnt);
}

//����ת���ɡ���ʾ
inline CString ConvertNegative(const CString value1)
{
    return SIGN_LEFT_PARENTHESIS + value1 + SIGN_NONCONJ + value1 + SIGN_RIGHT_PARENTHESIS;
}

//����ת���ɡ���ʾ
inline CString ConvertConjunction(const CString value1, const CString value2)
{
    return ConvertNegative(SIGN_LEFT_PARENTHESIS + value1 + SIGN_NONCONJ + value2 + SIGN_RIGHT_PARENTHESIS);
}

//����ת���ɡ���ʾ
inline CString ConvertDisjunction(const CString value1, const CString value2)
{
    return SIGN_LEFT_PARENTHESIS + ConvertNegative(value1) + SIGN_NONCONJ + ConvertNegative(value2) + SIGN_RIGHT_PARENTHESIS;
}

//����ת���ɡ���ʾ
inline CString ConvertInclusion(const CString value1, const CString value2)
{
    return ConvertDisjunction(ConvertNegative(value1), value2);
}

//���ȼ������ת���ɡ���ʾ
inline CString ConvertEquivalence(const CString value1, const CString value2)
{
    return ConvertConjunction(ConvertInclusion(value1, value2), ConvertInclusion(value2, value1));
}

//��ջ�������ת���ɡ���ʾ
inline void ConvertOnce(int op[], int &opCnt, CString value[], int &valueCnt)
{
    int &calcOp = op[--opCnt];
    CString &value1 = value[valueCnt - 2];
    CString &value2 = value[valueCnt - 1];
    if (calcOp == PRI_NEGATIVE)
    {
        value2 = ConvertNegative(value2);
    }
    else if (calcOp == PRI_CONJUNCTION)
    {
        value1 = ConvertConjunction(value1, value2);
        valueCnt--;
    }
    else if (calcOp == PRI_DISJUNCTION)
    {
        value1 = ConvertDisjunction(value1, value2);
        valueCnt--;
    }
    else if (calcOp == PRI_INCLUSION)
    {
        value1 = ConvertInclusion(value1, value2);
        valueCnt--;
    }
    else if (calcOp == PRI_EQUIVALENCE)
    {
        value1 = ConvertEquivalence(value1, value2);
        valueCnt--;
    }
}

//������ʽ�ġ���ֵ���ʽ
inline CString ConvertExpression(const CString variable[])
{
    int op[MAXCONTENT]; //�����ջ
    int opCnt = 0;
    CString value[MAXCONTENT]; //ת���ɵĵ�ֵ���ʽջ
    int valueCnt = 0;

    for (int i = 0; i < inputCnt; i++)
    {
        if (inputType[i] == TYPE_VAR)
        {
            value[valueCnt++] = variable[inputContent[i]];
        }
        else// if (inputType[i] == TYPE_OP)
        {
            if (inputContent[i] == PRI_RIGHT_PARENTHESIS)
            {
                while (op[opCnt - 1] != PRI_LEFT_PARENTHESIS)
                {
                    ConvertOnce(op, opCnt, value, valueCnt);
                    if (value[valueCnt - 1].GetLength() > MAX_ALLOW_LENGTH)
                    {
                        return _T("The answer is too long.");
                    }
                }
                opCnt--;
            }
            else if (inputContent[i] == PRI_LEFT_PARENTHESIS)
            {
                op[opCnt++] = inputContent[i];
            }
            else if (inputContent[i] == PRI_NEGATIVE)
            {
                op[opCnt++] = inputContent[i];
            }
            else
            {
                while (CmpPriority(op[opCnt - 1], inputContent[i]))
                {
                    ConvertOnce(op, opCnt, value, valueCnt);
                    if (value[valueCnt - 1].GetLength() > MAX_ALLOW_LENGTH)
                    {
                        return _T("The answer is too long.");
                    }
                }
                op[opCnt++] = inputContent[i];
            }
        }
    }
    return value[0];
}

//������ֵ��ͱ��ʽ�ġ���ֵ���ʽ
bool CalcTruthTable(CString sInput, CString &sTruthTable, int &varNumberCnt, CString variable[], CString &sConvertFormula)
{
    //variableΪ������ŵ���������ӳ��
	sInput = SIGN_LEFT_PARENTHESIS + sInput + SIGN_RIGHT_PARENTHESIS; //��ʽǰ��ֱ����()��������
    if (!DealInput(sInput, varNumberCnt, variable))
    {
        return false;
    }
    Dfs(0, 0, sTruthTable, varNumberCnt);
    sConvertFormula = ConvertExpression(variable);
    return true;
}

//������ֵ���жϹ�ʽ����
CString JudgeType(const CString &sTruthTable)
{
    bool existTrue = false, existFalse = false;
    for (int i = 0; i < sTruthTable.GetLength(); i++)
    {
        if (sTruthTable[i] == '1')
        {
            existTrue = true;
        }
        else
        {
            existFalse = true;
        }
    }
    if (!existFalse)
    {
        return _T("����ʽ");
    }
    if (!existTrue)
    {
        return _T("����ʽ");
    }
    return _T("������ʽ");
}

//intתΪCString
CString intToCString(int x)
{
    CString s("");
    do
    {
        s = (TCHAR)('0' + x % 10) + s;
        x /= 10;
    }
    while (x > 0);
    return s;
}

//������ֵ��������ȡ��ʽ������ȡ��ʽ
bool JudgePDNFAndPCNF(const CString &sTruthTable, CString &sPDNF, CString &sPCNF)
{
    int lastPDNF = -1, lastPCNF = -1;
    for (int i = 0; i < sTruthTable.GetLength(); i++)
    {
        if (sTruthTable[i] == '1')
        {
            if (sPDNF.IsEmpty())
            {
                sPDNF = _T("m") + intToCString(i);
            }
            else if (sPDNF.GetLength() > MAX_ALLOW_LENGTH)
            {
                lastPDNF = i;
            }
            else
            {
                sPDNF += SIGN_DISJUNCTION;
                sPDNF += _T("m") + intToCString(i);
            }
        }
        else
        {
            if (sPCNF.IsEmpty())
            {
                sPCNF = _T("M") + intToCString(i);
            }
            else if (sPCNF.GetLength() > MAX_ALLOW_LENGTH)
            {
                lastPCNF = i;
            }
            else
            {
                sPCNF += SIGN_CONJUNCTION;
                sPCNF += _T("M") + intToCString(i);
            }
        }
    }
    if (sPDNF.IsEmpty())
    {
        sPDNF = _T("0");
    }
    if (sPCNF.IsEmpty())
    {
        sPCNF = _T("1");
    }
    if (lastPDNF != -1)
    {
        sPDNF += SIGN_DISJUNCTION;
        sPDNF += _T("...");
        sPDNF += SIGN_DISJUNCTION;
        sPDNF += _T("m") + intToCString(lastPDNF);
    }
    if (lastPCNF != -1)
    {
        sPCNF += SIGN_CONJUNCTION;
        sPCNF += _T("...");
        sPCNF += SIGN_CONJUNCTION;
        sPCNF += _T("M") + intToCString(lastPCNF);
    }
    return true;
}
