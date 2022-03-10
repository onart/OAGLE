/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Hangul.h"

namespace onart {
    constexpr char16_t H_HEAD[] = u"��������������������������������������";
    constexpr char16_t H_MID[] = u"�������¤äĤŤƤǤȤɤʤˤ̤ͤΤϤФѤҤ�";
    constexpr char16_t H_TAIL[] = u" ������������������������������������������������������";
    constexpr char16_t H_2[] = u"���������������������ȤɤʤͤΤϤ�";
    constexpr size_t NOTFOUND = -1;

    constexpr bool isPiece(char16_t c) {
        return c >= u'��' && c <= u'��';
    }
    
    constexpr int checkPiece(char16_t c) {
        if (c >= u'��' && c <= u'��') return 1;
        else if (c >= u'��' && c <= u'��')return -1;
        else return 0;
    }
    
    void clearStack(std::stack<char16_t>& t) {
        std::stack<char16_t> sw; t.swap(sw);
    }

    size_t binSearch(char16_t c, const char16_t* ar, size_t high) {
        size_t low = 0;
        while (low <= high) {
            size_t mid = (low + high) / 2;
            if (ar[mid] == c)return mid;
            if (ar[mid] > c) high = mid - 1;
            else low = mid + 1;
        }
        return NOTFOUND;
    }

    size_t hIndex(char16_t c) {
        return binSearch(c, H_HEAD, 18);
    }

    size_t mIndex(char16_t c) {
        return binSearch(c, H_MID, 20);
    }

    size_t tIndex(char16_t c) {
        return binSearch(c, H_TAIL, 27);
    }

    size_t twIndex(char16_t c) {
        return binSearch(c, H_2, 16);
    }

    /// <summary>
    /// ����+����
    /// </summary>
    char16_t cpc(char a, char b) {
        switch (a)
        {
        case u'��':
            if (b == u'��') return u'��';
            break;
        case u'��':
            if (b == u'��') return u'��';
            else if (b == u'��')return u'��';
            break;
        case u'��':
            switch (b)
            {
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            }
            break;
        case u'��':
            if (b == '��')return '��';
        }
        return b;
    }

    /// <summary>
    /// ����+����
    /// </summary>
    char16_t vpv(char a, char b) {
        switch (a)
        {
        case u'��':
            switch (b)
            {
            case u'��': return u'��';
            case u'��': return u'��';
            case u'��': return u'��';
            }
            break;
        case u'��':
            switch (b) {
                case u'��': return u'��';
                case u'��': return u'��';
                case u'��': return u'��';
            }
            break;
        case u'��':
            if (b == u'��') return u'��';
        }
        return b;
    }

    /// <summary>
    /// ����+����
    /// </summary>
    char16_t cpv(char c, char v) {
        return u'��' + hIndex(c) * 588 + mIndex(v) * 28;
    }

    /// <summary>
    /// ����+��ħ
    /// </summary>
    char16_t lpc(char l, char c) {
        return l + tIndex(c);
    }

    void HangulStateMachine::push(char16_t c) {
        if (c == u'\b') {
            if (content.size() == 0) return;
            if (buf.size() > 0) {
                buf.pop();
                prev.pop();
                if (prev.top()) {
                    content[cursor] = prev.top();
                    switch (st)
                    {
                    case hState::R:
                    case hState::K:
                        st = hState::EMPTY;
                        break;
                    case onart::HangulStateMachine::ML:
                        st = hState::K;
                        break;
                    case onart::HangulStateMachine::RT:
                    case onart::HangulStateMachine::RK:
                        st = hState::R;
                        break;
                    case onart::HangulStateMachine::RML:
                    case onart::HangulStateMachine::RKR:
                        st = hState::RK;
                        break;
                    case onart::HangulStateMachine::RKFR:
                        st = hState::RKR;
                        break;
                    case onart::HangulStateMachine::RMLR:
                        st = hState::RML;
                        break;
                    case onart::HangulStateMachine::RMLFR:
                        st = hState::RMLR;
                        break;
                    }
                }
                else {
                    content.erase(cursor);
                }
            }
            else {
                content.erase(--cursor);
            }
            return;
        }
        else if (c == 0) {
            clearBuffers();
            cursor++;
            return;
        }
        int typ = checkPiece(c);
        if (typ == 0) {
            if (prev.top()) {
                content.insert(++cursor, 1, c);
                cursor++;
                clearBuffers();
            }
            else {
                content.insert(cursor++, 1, c);
            }
            return;
        }
        else {
            switch (st)
            {
            case hState::EMPTY:
                prev.push(c);
                buf.push(c);
                if (typ == 1) { st = hState::R; }
                else { st = hState::K; }
                break;
            case hState::R:
                if (typ == 1) { // ����+���� �õ�
                    char16_t nw = cpc(prev.top(), c);
                    if (nw == c) {
                        buf.top() = c;
                        prev.top() = c;
                        content.insert(++cursor, 1, c);
                    }
                    else {
                        buf.push(c);
                        prev.push(nw);
                        st = hState::RT;
                        content[cursor] = nw;
                    }
                }
                else {  // ����+����
                    char16_t nw = cpv(prev.top(), c);
                    buf.push(c);
                    prev.push(nw);
                    content[cursor] = nw;
                    st = hState::RK;
                }
                break;
            case hState::K:
                if (typ == 1) { // ���� ����
                    buf.top() = c;
                    prev.top() = c;
                    content.insert(++cursor, 1, c);
                    st = hState::R;
                }
                else {  // ����+���� �õ�
                    char16_t nw = vpv(prev.top(), c);
                    if (nw == c) {
                        buf.top() = c;
                        prev.top() = c;
                        content.insert(++cursor, 1, c);
                    }
                    else {
                        buf.push(c);
                        prev.push(nw);
                        content[cursor] = nw;
                        st = hState::ML;
                    }
                }
                break;
            case hState::ML:
                content.insert(++cursor, 1, c);
                clearBuffers();
                buf.push(c);
                prev.push(c);
                if (typ == 1) st = hState::R;   // ���� ����
                else st = hState::K;            // ���� ����
                break;
            case hState::RT:
                if (typ == 1) { // ���� ����
                    clearBuffers();
                    buf.push(c);
                    prev.push(c);
                    content.insert(++cursor, 1, c);
                    st = hState::R;
                }
                else {  // �� �������� �ϳ� ���� ��ħ
                    char16_t latter = buf.top(); buf.pop();
                    char16_t former = buf.top();
                    content[cursor++] = former;
                    clearBuffers();
                    buf.push(latter);
                    buf.push(c);
                    char16_t nw = cpv(latter, c);
                    prev.push(latter);
                    prev.push(nw);
                    content.insert(cursor, 1, nw);
                    st = hState::RK;
                }
                break;
            case hState::RK:
                if (typ == 1) { // ��ħ �õ�
                    if (tIndex(c) != NOTFOUND) {
                        clearBuffers();
                        buf.push(c);
                        prev.push(c);
                        st = hState::R;
                    }
                    else {
                        char16_t nw = lpc(prev.top(), c);
                        buf.push(c);
                        prev.push(nw);
                        content[cursor] = nw;
                        st = hState::RKR;
                    }
                }
                else {  // ���� ��ü �õ�
                    char16_t nw = vpv(buf.top(), c);
                    if (nw == c) {  // ����
                        clearBuffers();
                        buf.push(c);
                        prev.push(c);
                        st = hState::K;
                    }
                    else {
                        buf.push(c);
                        char16_t now = prev.top(); prev.pop();
                        char16_t next = cpv(prev.top(), nw);
                        prev.push(now);
                        prev.push(next);
                        st = hState::RML;
                    }
                }
                break;
            case hState::RML:
                if (typ == 1) { // ��ħ �õ�
                    if (tIndex(c) != NOTFOUND) {
                        clearBuffers();
                        content.insert(++cursor, 1, c);
                        buf.push(c);
                        prev.push(c);
                        st = hState::R;
                    }
                    else {
                        char16_t nw = lpc(prev.top(), c);
                        buf.push(c);
                        prev.push(nw);
                        content[cursor] = nw;
                        st = hState::RMLR;
                    }
                }
                else {  // ���� ����
                    clearBuffers();
                    content.insert(++cursor, 1, c);
                    buf.push(c);
                    prev.push(c);
                    st = hState::K;
                }
                break;
            case hState::RKR:
                if (typ == 1) {
                    char16_t fr = buf.top();
                    char16_t nw = cpc(fr, c);
                    if (nw == c) {  // ���� ����
                        clearBuffers();
                        buf.push(c);
                        prev.push(c);
                        content.insert(++cursor, 1, c);
                        st = hState::R;
                    }
                    else {  // ���� ��ħ
                        char16_t former = prev.top(); prev.pop();
                        char16_t next = lpc(prev.top(), nw);
                        prev.push(former);
                        prev.push(next);
                        buf.push(c);
                        content[cursor] = next;
                        st = hState::RKFR;
                    }
                }
                else {  // ����
                    char16_t nc = buf.top();
                    char16_t next = cpv(nc, c);
                    prev.pop();
                    content[++cursor] = prev.top();
                    clearBuffers();
                    content.insert(cursor, 1, next);
                    buf.push(nc);
                    buf.push(c);
                    prev.push(nc);
                    prev.push(next);
                    st = hState::RK;
                }
                break;
            case hState::RKFR:
                if (typ == 1) { // ���� ����
                    clearBuffers();
                    buf.push(c);
                    prev.push(c);
                    content.insert(++cursor, 1, c);
                    st = hState::R;
                }
                else {  // ����
                    char16_t nc = buf.top();
                    char16_t next = cpv(nc, c);
                    prev.pop();
                    content[++cursor] = prev.top();
                    clearBuffers();
                    content.insert(cursor, 1, next);
                    buf.push(nc);
                    buf.push(c);
                    prev.push(nc);
                    prev.push(next);
                    st = hState::RK;
                }
                break;
            case hState::RMLR:
                if (typ == 1) {
                    char nw = cpc(buf.top(), c);
                    if (nw == c) {  // ���� ����
                        clearBuffers();
                        buf.push(c);
                        prev.push(c);
                        content.insert(++cursor, 1, c);
                        st = hState::R;
                    }
                    else {  // ����+����
                        char16_t former = prev.top(); prev.pop();
                        char16_t next = lpc(prev.top(), nw);
                        prev.push(former);
                        prev.push(next);
                        buf.push(c);
                        content[cursor] = next;
                        st = hState::RMLFR;
                    }
                }
                else {  // ����
                    char16_t nc = buf.top();
                    char16_t next = cpv(nc, c);
                    prev.pop();
                    content[++cursor] = prev.top();
                    clearBuffers();
                    content.insert(cursor, 1, next);
                    buf.push(nc);
                    buf.push(c);
                    prev.push(nc);
                    prev.push(next);
                    st = hState::RK;
                }
                break;
            case hState::RMLFR:
                if (typ == 1) { // ���� ����
                    clearBuffers();
                    buf.push(c);
                    prev.push(c);
                    content.insert(++cursor, 1, c);
                    st = hState::R;
                }
                else {  // ����
                    char16_t nc = buf.top();
                    char16_t next = cpv(nc, c);
                    prev.pop();
                    content[++cursor] = prev.top();
                    clearBuffers();
                    content.insert(cursor, 1, next);
                    buf.push(nc);
                    buf.push(c);
                    prev.push(nc);
                    prev.push(next);
                    st = hState::RK;
                }
                break;
            default:
                break;
            }
        }
    }

    void HangulStateMachine::flush(const std::u16string& content = u"") {
        this->content = content;
        cursor = content.size();
        clearBuffers();
    }

    void HangulStateMachine::append(const std::u16string& str) {
        if (prev.top()) {
            content.insert(cursor + 1, str);
            cursor += 1 + str.size();
            clearBuffers();
        }
        else {
            content.insert(cursor, str);
            cursor += str.size();
            clearBuffers();
        }
    }

    void HangulStateMachine::home() {
        clearBuffers();
        cursor = 0;
    }

    void HangulStateMachine::end() {
        clearBuffers();
        cursor = content.size();
    }

    void HangulStateMachine::clearBuffers() {
        clearStack(buf);
        clearStack(prev);
        prev.push(0);
        st = hState::EMPTY;
    }

    void HangulStateMachine::moveCursor(ptrdiff_t displace) {
        ptrdiff_t cs = (ptrdiff_t)cursor;
        cs += displace;
        if (cs < 0)cs = 0;
        else if (cs > content.size()) cs = content.size();
        cursor = (size_t)cs;
    }
}