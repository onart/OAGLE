/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Hangul.h"

namespace onart {
    constexpr size_t H_HEAD[] = { 0, 1, -1, 2, -1, -1, 3, 4, 5, -1, -1, -1, -1, -1, -1, -1, 6, 7, 8, -1, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
    constexpr size_t H_TAIL[] = { 1, 2, 3, 4, 5, 6, 7, -1, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, -1, 18, 19, 20, 21, 22, -1, 23, 24, 25, 26, 27 };
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

    size_t hIndex(char16_t c) {
        c -= u'��';
        if (c < 0 || c>29)return NOTFOUND;
        return H_HEAD[c];
    }

    size_t mIndex(char16_t c) {
        c -= u'��';
        if (c < 0 || c>20)return NOTFOUND;
        return c;
    }

    size_t tIndex(char16_t c) {
        c -= u'��';
        if (c < 0 || c>29)return NOTFOUND;
        return H_TAIL[c];
    }

    /// <summary>
    /// ����+����
    /// </summary>
    char16_t cpc(char16_t a, char16_t b) {
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
    char16_t vpv(char16_t a, char16_t b) {
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
    char16_t cpv(char16_t c, char16_t v) {
        return u'��' + (char16_t)hIndex(c) * 588 + (char16_t)mIndex(v) * 28;
    }

    /// <summary>
    /// ����+��ħ
    /// </summary>
    char16_t lpc(char16_t l, char16_t c) {
        return l + (char16_t)tIndex(c);
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
                    case hState::ML:
                        st = hState::K;
                        break;
                    case hState::RT:
                    case hState::RK:
                        st = hState::R;
                        break;
                    case hState::RML:
                    case hState::RKR:
                        st = hState::RK;
                        break;
                    case hState::RKFR:
                        st = hState::RKR;
                        break;
                    case hState::RMLR:
                        st = hState::RML;
                        break;
                    case hState::RMLFR:
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
                    char16_t nw = cpc(buf.top(), c);
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

    void HangulStateMachine::flush(const std::u16string& content) {
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
        else if (cs > (ptrdiff_t)content.size()) cs = content.size();
        cursor = (size_t)cs;
    }
}