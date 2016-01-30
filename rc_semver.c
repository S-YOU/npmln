#include "Python.h"
#include "src/base.h"
#include "src/conv.h"

str M1, M2, M3, M4, M5, M6;
str *starts, *ends;

/*
./rc '(?x)
#>= v1.2.3-alpha.10.beta.0+build.unicorn.rainbow
([<>]=|[<->~^])?        #< <= > >= ~ ^ =
\s*
v?(?:(?I:\d+)|\*)      #1
(?:\.(?I:\d+)|\.x)?    #2
({4})?                 #3
(?:-(\w+(?:\.\w+)*))?  #alpha.10.beta.0
(?:\+([\w.]+))?        #build.unicorn.rainbow
' semver
*/

static PyObject* test(PyObject* self, PyObject* arg) {
	unsigned i;
	unsigned j;
	if (!PyString_CheckExact(arg)) return NULL;
	str src = (str) PyString_AS_STRING(arg);
	unsigned len = PyString_GET_SIZE(arg);
	if (len <= 0) Py_RETURN_NONE;
	str s = src, sEnd = src + len - 0;
	while (s < sEnd) {
		i = 0;
		//group 1 start
		j = i; //push pos
		//char class
		if (!M1[s[i++]]) goto loc_2;

		//literal
		if (s[i++] != 61) goto loc_2;
		//matched
		goto grp_1_done;

	loc_2:
		i = j; //pop pos
		//char class
		if (!M2[s[i++]]) goto grp_1_fail;
		//group 1 end

		goto grp_1_done;

	grp_1_fail:
		i = j; //restore if not matched

	grp_1_done:
		//group 1 matched

		//char class
		while (M3[s[i]]) i++;

		//literal
		if (s[i] == 118) i++;
		//group 2 start
		j = i; //push pos
		//group 3 start
		j = i; //push pos
		//char class
		if (!M4[s[i++]]) goto loc_7;
		while (M4[s[i]]) i++;
		//group 3 end
		//group 3 matched

		//matched
		goto grp_2_done;

	loc_7:
		i = j; //pop pos

		//literal
		if (s[i++] != 42) goto grp_2_fail;
		//group 2 end

		goto grp_2_done;

	grp_2_fail:
		goto next;

	grp_2_done:
		//group 2 matched

		//group 4 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_12;
		//group 5 start
		j = i; //push pos
		//char class
		if (!M4[s[i++]]) goto loc_12;
		while (M4[s[i]]) i++;
		//group 5 end
		//group 5 matched

		//matched
		goto grp_4_done;

	loc_12:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_4_fail;

		//literal
		if (s[i++] != 120) goto grp_4_fail;
		//group 4 end

		goto grp_4_done;

	grp_4_fail:
		i = j; //restore if not matched

	grp_4_done:
		//group 4 matched

		//group 6 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_17;
		//group 7 start
		j = i; //push pos
		//char class
		if (!M4[s[i++]]) goto loc_17;
		while (M4[s[i]]) i++;
		//group 7 end
		//group 7 matched

		//matched
		goto grp_6_done;

	loc_17:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_6_fail;

		//literal
		if (s[i++] != 120) goto grp_6_fail;
		//group 6 end

		goto grp_6_done;

	grp_6_fail:
		i = j; //restore if not matched

	grp_6_done:
		//group 6 matched

		//group 8 start
		j = i; //push pos

		//literal
		if (s[i++] != 45) goto grp_8_fail;
		//group 9 start
		j = i; //push pos
		//char class
		if (!M5[s[i++]]) goto grp_9_fail;
		while (M5[s[i]]) i++;
	for(;;) {
		//group 10 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto grp_10_fail;
		//char class
		if (!M5[s[i++]]) goto grp_10_fail;
		while (M5[s[i]]) i++;
		//group 10 end

		goto grp_10_done;

	grp_10_fail:
		i = j;
		break;
	grp_10_done:
	;}
		//group 10 matched

		//group 9 end

		goto grp_9_done;

	grp_9_fail:
		goto next;

	grp_9_done:
		//group 9 matched

		//group 8 end

		goto grp_8_done;

	grp_8_fail:
		i = j; //restore if not matched

	grp_8_done:
		//group 8 matched

		//group 11 start
		j = i; //push pos

		//literal
		if (s[i++] != 43) goto grp_11_fail;
		//group 12 start
		j = i; //push pos
		//char class
		if (!M6[s[i++]]) goto grp_12_fail;
		while (M6[s[i]]) i++;
		//group 12 end

		goto grp_12_done;

	grp_12_fail:
		goto next;

	grp_12_done:
		//group 12 matched

		//group 11 end

		goto grp_11_done;

	grp_11_fail:
		i = j; //restore if not matched

	grp_11_done:
		//group 11 matched


		//matched
		Py_RETURN_TRUE;
	next:
		s++;
	}
	Py_RETURN_FALSE;
}

static PyObject* findone(PyObject* self, PyObject* arg) {
	unsigned i;
	unsigned j;
	if (!PyString_CheckExact(arg)) return NULL;
	str src = (str) PyString_AS_STRING(arg);
	unsigned len = PyString_GET_SIZE(arg);
	if (len <= 0) Py_RETURN_NONE;
	PyObject *item;
	PyObject *val;
	memset(starts, 0, sizeof(void*) * 14);
	str sTmp; unsigned mul, l; long num;
	str s = src, sEnd = src + len - 0;
	while (s < sEnd) {
		i = 0;
		//group 1 start
		j = i; //push pos

		starts[1] = s + j; //store starts
		//char class
		if (!M1[s[i++]]) goto loc_2;

		//literal
		if (s[i++] != 61) goto loc_2;
		//matched
		goto grp_1_done;

	loc_2:
		i = j; //pop pos
		//char class
		if (!M2[s[i++]]) goto grp_1_fail;
		//group 1 end

		goto grp_1_done;

	grp_1_fail:
		i = j; //restore if not matched

	grp_1_done:
		//group 1 matched

		ends[1] = s + i; //store ends

		//char class
		while (M3[s[i]]) i++;

		//literal
		if (s[i] == 118) i++;
		//group 2 start
		j = i; //push pos
		//group 3 start
		j = i; //push pos

		starts[2] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_7;
		while (M4[s[i]]) i++;
		//group 3 end
		//group 3 matched

		ends[2] = s + i; //store ends

		//matched
		goto grp_2_done;

	loc_7:
		i = j; //pop pos

		//literal
		if (s[i++] != 42) goto grp_2_fail;
		//group 2 end

		goto grp_2_done;

	grp_2_fail:
		goto next;

	grp_2_done:
		//group 2 matched

		//group 4 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_12;
		//group 5 start
		j = i; //push pos

		starts[3] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_12;
		while (M4[s[i]]) i++;
		//group 5 end
		//group 5 matched

		ends[3] = s + i; //store ends

		//matched
		goto grp_4_done;

	loc_12:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_4_fail;

		//literal
		if (s[i++] != 120) goto grp_4_fail;
		//group 4 end

		goto grp_4_done;

	grp_4_fail:
		i = j; //restore if not matched

	grp_4_done:
		//group 4 matched

		//group 6 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_17;
		//group 7 start
		j = i; //push pos

		starts[4] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_17;
		while (M4[s[i]]) i++;
		//group 7 end
		//group 7 matched

		ends[4] = s + i; //store ends

		//matched
		goto grp_6_done;

	loc_17:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_6_fail;

		//literal
		if (s[i++] != 120) goto grp_6_fail;
		//group 6 end

		goto grp_6_done;

	grp_6_fail:
		i = j; //restore if not matched

	grp_6_done:
		//group 6 matched

		//group 8 start
		j = i; //push pos

		//literal
		if (s[i++] != 45) goto grp_8_fail;
		//group 9 start
		j = i; //push pos

		starts[5] = s + j; //store starts
		//char class
		if (!M5[s[i++]]) goto grp_9_fail;
		while (M5[s[i]]) i++;
	for(;;) {
		//group 10 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto grp_10_fail;
		//char class
		if (!M5[s[i++]]) goto grp_10_fail;
		while (M5[s[i]]) i++;
		//group 10 end

		goto grp_10_done;

	grp_10_fail:
		i = j;
		break;
	grp_10_done:
	;}
		//group 10 matched

		//group 9 end

		goto grp_9_done;

	grp_9_fail:
		goto next;

	grp_9_done:
		//group 9 matched

		ends[5] = s + i; //store ends

		//group 8 end

		goto grp_8_done;

	grp_8_fail:
		i = j; //restore if not matched

	grp_8_done:
		//group 8 matched

		//group 11 start
		j = i; //push pos

		//literal
		if (s[i++] != 43) goto grp_11_fail;
		//group 12 start
		j = i; //push pos

		starts[6] = s + j; //store starts
		//char class
		if (!M6[s[i++]]) goto grp_12_fail;
		while (M6[s[i]]) i++;
		//group 12 end

		goto grp_12_done;

	grp_12_fail:
		goto next;

	grp_12_done:
		//group 12 matched

		ends[6] = s + i; //store ends

		//group 11 end

		goto grp_11_done;

	grp_11_fail:
		i = j; //restore if not matched

	grp_11_done:
		//group 11 matched


		s += i; //skip starts

		//matched
		item = PyTuple_New(6);

		l = ends[1] ? ends[1] - starts[1]: 0;
		val = PyString_FromStringAndSize((char*)starts[1], l);
		starts[1] = ends[1] = 0;
		PyTuple_SET_ITEM(item, 0, val);

		if (ends[2]) {
			ATOI(starts[2], ends[2], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[2] = ends[2] = 0;
		PyTuple_SET_ITEM(item, 1, val);

		if (ends[3]) {
			ATOI(starts[3], ends[3], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[3] = ends[3] = 0;
		PyTuple_SET_ITEM(item, 2, val);

		if (ends[4]) {
			ATOI(starts[4], ends[4], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[4] = ends[4] = 0;
		PyTuple_SET_ITEM(item, 3, val);

		l = ends[5] ? ends[5] - starts[5]: 0;
		val = PyString_FromStringAndSize((char*)starts[5], l);
		starts[5] = ends[5] = 0;
		PyTuple_SET_ITEM(item, 4, val);

		l = ends[6] ? ends[6] - starts[6]: 0;
		val = PyString_FromStringAndSize((char*)starts[6], l);
		starts[6] = ends[6] = 0;
		PyTuple_SET_ITEM(item, 5, val);


		return item;
	next:
		s++;
	}
	Py_RETURN_NONE;
}

static PyObject* findall(PyObject* self, PyObject* arg) {
	unsigned i;
	unsigned j;
	PyTupleObject* argTuple = (PyTupleObject*) arg;
	if (Py_SIZE(argTuple) != 2 || !PyString_CheckExact(argTuple->ob_item[0]) || !PyInt_CheckExact(argTuple->ob_item[1])) return NULL;
	str src = (str) PyString_AS_STRING(argTuple->ob_item[0]);
	unsigned len = PyString_GET_SIZE(argTuple->ob_item[0]), max = PyInt_AsSsize_t(argTuple->ob_item[1]);
	if (len <= 0 || !max) Py_RETURN_NONE;
	PyObject *list = PyList_New(max); unsigned index = 0;
	PyObject *item;
	PyObject *val;
	memset(starts, 0, sizeof(void*) * 14);
	str sTmp; unsigned mul, l; long num;
	str s = src, sEnd = src + len - 0;
	while (s < sEnd) {
		i = 0;
		//group 1 start
		j = i; //push pos

		starts[1] = s + j; //store starts
		//char class
		if (!M1[s[i++]]) goto loc_2;

		//literal
		if (s[i++] != 61) goto loc_2;
		//matched
		goto grp_1_done;

	loc_2:
		i = j; //pop pos
		//char class
		if (!M2[s[i++]]) goto grp_1_fail;
		//group 1 end

		goto grp_1_done;

	grp_1_fail:
		i = j; //restore if not matched

	grp_1_done:
		//group 1 matched

		ends[1] = s + i; //store ends

		//char class
		while (M3[s[i]]) i++;

		//literal
		if (s[i] == 118) i++;
		//group 2 start
		j = i; //push pos
		//group 3 start
		j = i; //push pos

		starts[2] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_7;
		while (M4[s[i]]) i++;
		//group 3 end
		//group 3 matched

		ends[2] = s + i; //store ends

		//matched
		goto grp_2_done;

	loc_7:
		i = j; //pop pos

		//literal
		if (s[i++] != 42) goto grp_2_fail;
		//group 2 end

		goto grp_2_done;

	grp_2_fail:
		goto next;

	grp_2_done:
		//group 2 matched

		//group 4 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_12;
		//group 5 start
		j = i; //push pos

		starts[3] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_12;
		while (M4[s[i]]) i++;
		//group 5 end
		//group 5 matched

		ends[3] = s + i; //store ends

		//matched
		goto grp_4_done;

	loc_12:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_4_fail;

		//literal
		if (s[i++] != 120) goto grp_4_fail;
		//group 4 end

		goto grp_4_done;

	grp_4_fail:
		i = j; //restore if not matched

	grp_4_done:
		//group 4 matched

		//group 6 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_17;
		//group 7 start
		j = i; //push pos

		starts[4] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_17;
		while (M4[s[i]]) i++;
		//group 7 end
		//group 7 matched

		ends[4] = s + i; //store ends

		//matched
		goto grp_6_done;

	loc_17:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_6_fail;

		//literal
		if (s[i++] != 120) goto grp_6_fail;
		//group 6 end

		goto grp_6_done;

	grp_6_fail:
		i = j; //restore if not matched

	grp_6_done:
		//group 6 matched

		//group 8 start
		j = i; //push pos

		//literal
		if (s[i++] != 45) goto grp_8_fail;
		//group 9 start
		j = i; //push pos

		starts[5] = s + j; //store starts
		//char class
		if (!M5[s[i++]]) goto grp_9_fail;
		while (M5[s[i]]) i++;
	for(;;) {
		//group 10 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto grp_10_fail;
		//char class
		if (!M5[s[i++]]) goto grp_10_fail;
		while (M5[s[i]]) i++;
		//group 10 end

		goto grp_10_done;

	grp_10_fail:
		i = j;
		break;
	grp_10_done:
	;}
		//group 10 matched

		//group 9 end

		goto grp_9_done;

	grp_9_fail:
		goto next;

	grp_9_done:
		//group 9 matched

		ends[5] = s + i; //store ends

		//group 8 end

		goto grp_8_done;

	grp_8_fail:
		i = j; //restore if not matched

	grp_8_done:
		//group 8 matched

		//group 11 start
		j = i; //push pos

		//literal
		if (s[i++] != 43) goto grp_11_fail;
		//group 12 start
		j = i; //push pos

		starts[6] = s + j; //store starts
		//char class
		if (!M6[s[i++]]) goto grp_12_fail;
		while (M6[s[i]]) i++;
		//group 12 end

		goto grp_12_done;

	grp_12_fail:
		goto next;

	grp_12_done:
		//group 12 matched

		ends[6] = s + i; //store ends

		//group 11 end

		goto grp_11_done;

	grp_11_fail:
		i = j; //restore if not matched

	grp_11_done:
		//group 11 matched


		s += i; //skip starts

		//matched
		item = PyTuple_New(6);

		l = ends[1] ? ends[1] - starts[1]: 0;
		val = PyString_FromStringAndSize((char*)starts[1], l);
		starts[1] = ends[1] = 0;
		PyTuple_SET_ITEM(item, 0, val);

		if (ends[2]) {
			ATOI(starts[2], ends[2], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[2] = ends[2] = 0;
		PyTuple_SET_ITEM(item, 1, val);

		if (ends[3]) {
			ATOI(starts[3], ends[3], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[3] = ends[3] = 0;
		PyTuple_SET_ITEM(item, 2, val);

		if (ends[4]) {
			ATOI(starts[4], ends[4], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[4] = ends[4] = 0;
		PyTuple_SET_ITEM(item, 3, val);

		l = ends[5] ? ends[5] - starts[5]: 0;
		val = PyString_FromStringAndSize((char*)starts[5], l);
		starts[5] = ends[5] = 0;
		PyTuple_SET_ITEM(item, 4, val);

		l = ends[6] ? ends[6] - starts[6]: 0;
		val = PyString_FromStringAndSize((char*)starts[6], l);
		starts[6] = ends[6] = 0;
		PyTuple_SET_ITEM(item, 5, val);


		PyList_SET_ITEM(list, index++, item);
		if (index >= max) return list;
	next:
		s++;
	}
	Py_SIZE(list) = index;
	return list;
}

typedef struct {
	PyObject_HEAD
	str src;
	str sEnd;
} iter_s;

PyObject* iter_next(iter_s *r) {
	str src = r->src, s = src, sEnd = r->sEnd;
	unsigned i;
	unsigned j;
	PyObject *item;
	PyObject *val;
	memset(starts, 0, sizeof(void*) * 14);
	str sTmp; unsigned mul, l; long num;
	while (s < sEnd) {
		i = 0;
		//group 1 start
		j = i; //push pos

		starts[1] = s + j; //store starts
		//char class
		if (!M1[s[i++]]) goto loc_2;

		//literal
		if (s[i++] != 61) goto loc_2;
		//matched
		goto grp_1_done;

	loc_2:
		i = j; //pop pos
		//char class
		if (!M2[s[i++]]) goto grp_1_fail;
		//group 1 end

		goto grp_1_done;

	grp_1_fail:
		i = j; //restore if not matched

	grp_1_done:
		//group 1 matched

		ends[1] = s + i; //store ends

		//char class
		while (M3[s[i]]) i++;

		//literal
		if (s[i] == 118) i++;
		//group 2 start
		j = i; //push pos
		//group 3 start
		j = i; //push pos

		starts[2] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_7;
		while (M4[s[i]]) i++;
		//group 3 end
		//group 3 matched

		ends[2] = s + i; //store ends

		//matched
		goto grp_2_done;

	loc_7:
		i = j; //pop pos

		//literal
		if (s[i++] != 42) goto grp_2_fail;
		//group 2 end

		goto grp_2_done;

	grp_2_fail:
		goto next;

	grp_2_done:
		//group 2 matched

		//group 4 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_12;
		//group 5 start
		j = i; //push pos

		starts[3] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_12;
		while (M4[s[i]]) i++;
		//group 5 end
		//group 5 matched

		ends[3] = s + i; //store ends

		//matched
		goto grp_4_done;

	loc_12:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_4_fail;

		//literal
		if (s[i++] != 120) goto grp_4_fail;
		//group 4 end

		goto grp_4_done;

	grp_4_fail:
		i = j; //restore if not matched

	grp_4_done:
		//group 4 matched

		//group 6 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_17;
		//group 7 start
		j = i; //push pos

		starts[4] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_17;
		while (M4[s[i]]) i++;
		//group 7 end
		//group 7 matched

		ends[4] = s + i; //store ends

		//matched
		goto grp_6_done;

	loc_17:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_6_fail;

		//literal
		if (s[i++] != 120) goto grp_6_fail;
		//group 6 end

		goto grp_6_done;

	grp_6_fail:
		i = j; //restore if not matched

	grp_6_done:
		//group 6 matched

		//group 8 start
		j = i; //push pos

		//literal
		if (s[i++] != 45) goto grp_8_fail;
		//group 9 start
		j = i; //push pos

		starts[5] = s + j; //store starts
		//char class
		if (!M5[s[i++]]) goto grp_9_fail;
		while (M5[s[i]]) i++;
	for(;;) {
		//group 10 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto grp_10_fail;
		//char class
		if (!M5[s[i++]]) goto grp_10_fail;
		while (M5[s[i]]) i++;
		//group 10 end

		goto grp_10_done;

	grp_10_fail:
		i = j;
		break;
	grp_10_done:
	;}
		//group 10 matched

		//group 9 end

		goto grp_9_done;

	grp_9_fail:
		goto next;

	grp_9_done:
		//group 9 matched

		ends[5] = s + i; //store ends

		//group 8 end

		goto grp_8_done;

	grp_8_fail:
		i = j; //restore if not matched

	grp_8_done:
		//group 8 matched

		//group 11 start
		j = i; //push pos

		//literal
		if (s[i++] != 43) goto grp_11_fail;
		//group 12 start
		j = i; //push pos

		starts[6] = s + j; //store starts
		//char class
		if (!M6[s[i++]]) goto grp_12_fail;
		while (M6[s[i]]) i++;
		//group 12 end

		goto grp_12_done;

	grp_12_fail:
		goto next;

	grp_12_done:
		//group 12 matched

		ends[6] = s + i; //store ends

		//group 11 end

		goto grp_11_done;

	grp_11_fail:
		i = j; //restore if not matched

	grp_11_done:
		//group 11 matched


		s += i; //skip starts

		//matched
		item = PyTuple_New(6);

		l = ends[1] ? ends[1] - starts[1]: 0;
		val = PyString_FromStringAndSize((char*)starts[1], l);
		starts[1] = ends[1] = 0;
		PyTuple_SET_ITEM(item, 0, val);

		if (ends[2]) {
			ATOI(starts[2], ends[2], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[2] = ends[2] = 0;
		PyTuple_SET_ITEM(item, 1, val);

		if (ends[3]) {
			ATOI(starts[3], ends[3], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[3] = ends[3] = 0;
		PyTuple_SET_ITEM(item, 2, val);

		if (ends[4]) {
			ATOI(starts[4], ends[4], num);
			val = PyInt_FromLong(num);
		} else val = Py_None;
		starts[4] = ends[4] = 0;
		PyTuple_SET_ITEM(item, 3, val);

		l = ends[5] ? ends[5] - starts[5]: 0;
		val = PyString_FromStringAndSize((char*)starts[5], l);
		starts[5] = ends[5] = 0;
		PyTuple_SET_ITEM(item, 4, val);

		l = ends[6] ? ends[6] - starts[6]: 0;
		val = PyString_FromStringAndSize((char*)starts[6], l);
		starts[6] = ends[6] = 0;
		PyTuple_SET_ITEM(item, 5, val);


		r->src = s;
		return item;
	next:
		s++;
	}
	r->src = sEnd;
	return NULL;
}

PyTypeObject IterType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,0,sizeof(iter_s),0,(destructor)PyObject_Del,0,0,0,0,0,0,0,0,0,0,0,PyObject_GenericGetAttr,0,0,Py_TPFLAGS_DEFAULT,0,0,0,0,0,PyObject_SelfIter,(iternextfunc)iter_next,0,0,0,0,0,0,0,0,0,0,0,
};

static PyObject* finditer(PyObject *type, PyObject *arg) {
	iter_s *obj = PyObject_New(iter_s, &IterType);
	if (obj == NULL) return NULL;
	if (!PyString_CheckExact(arg)) return NULL;
	obj->src = (str) PyString_AS_STRING(arg);
	obj->sEnd = obj->src + PyString_GET_SIZE(arg);
	return (PyObject *) obj;
}

static PyObject* sub(PyObject* self, PyObject* arg) {
	unsigned i;
	unsigned j;
	PyTupleObject* argTuple = (PyTupleObject*) arg;
	if (Py_SIZE(argTuple) != 2 || !PyString_CheckExact(argTuple->ob_item[0]) || !PyString_CheckExact(argTuple->ob_item[1])) return NULL;
	str src = (str) PyString_AS_STRING(argTuple->ob_item[0]), prev_s = src, repl = (str) PyString_AS_STRING(argTuple->ob_item[1]), dst, d;
	unsigned len = PyString_GET_SIZE(argTuple->ob_item[0]), rlen = PyString_GET_SIZE(argTuple->ob_item[1]);
	PyStringObject *ret = (PyStringObject*) PyString_FromStringAndSize(NULL, len * 2 + rlen * len + 1024); d = dst = (str) PyString_AS_STRING(ret);
	if (len <= 0) Py_RETURN_NONE;
	unsigned l;
	memset(starts, 0, sizeof(void*) * 14);
	str s = src, sEnd = src + len - 0;
	while (s < sEnd) {
		i = 0;
		starts[0] = s; //store $0
		//group 1 start
		j = i; //push pos

		starts[1] = s + j; //store starts
		//char class
		if (!M1[s[i++]]) goto loc_2;

		//literal
		if (s[i++] != 61) goto loc_2;
		//matched
		goto grp_1_done;

	loc_2:
		i = j; //pop pos
		//char class
		if (!M2[s[i++]]) goto grp_1_fail;
		//group 1 end

		goto grp_1_done;

	grp_1_fail:
		i = j; //restore if not matched

	grp_1_done:
		//group 1 matched

		ends[1] = s + i; //store ends

		//char class
		while (M3[s[i]]) i++;

		//literal
		if (s[i] == 118) i++;
		//group 2 start
		j = i; //push pos
		//group 3 start
		j = i; //push pos

		starts[2] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_7;
		while (M4[s[i]]) i++;
		//group 3 end
		//group 3 matched

		ends[2] = s + i; //store ends

		//matched
		goto grp_2_done;

	loc_7:
		i = j; //pop pos

		//literal
		if (s[i++] != 42) goto grp_2_fail;
		//group 2 end

		goto grp_2_done;

	grp_2_fail:
		goto next;

	grp_2_done:
		//group 2 matched

		//group 4 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_12;
		//group 5 start
		j = i; //push pos

		starts[3] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_12;
		while (M4[s[i]]) i++;
		//group 5 end
		//group 5 matched

		ends[3] = s + i; //store ends

		//matched
		goto grp_4_done;

	loc_12:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_4_fail;

		//literal
		if (s[i++] != 120) goto grp_4_fail;
		//group 4 end

		goto grp_4_done;

	grp_4_fail:
		i = j; //restore if not matched

	grp_4_done:
		//group 4 matched

		//group 6 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto loc_17;
		//group 7 start
		j = i; //push pos

		starts[4] = s + j; //store starts
		//char class
		if (!M4[s[i++]]) goto loc_17;
		while (M4[s[i]]) i++;
		//group 7 end
		//group 7 matched

		ends[4] = s + i; //store ends

		//matched
		goto grp_6_done;

	loc_17:
		i = j; //pop pos

		//literal
		if (s[i++] != 46) goto grp_6_fail;

		//literal
		if (s[i++] != 120) goto grp_6_fail;
		//group 6 end

		goto grp_6_done;

	grp_6_fail:
		i = j; //restore if not matched

	grp_6_done:
		//group 6 matched

		//group 8 start
		j = i; //push pos

		//literal
		if (s[i++] != 45) goto grp_8_fail;
		//group 9 start
		j = i; //push pos

		starts[5] = s + j; //store starts
		//char class
		if (!M5[s[i++]]) goto grp_9_fail;
		while (M5[s[i]]) i++;
	for(;;) {
		//group 10 start
		j = i; //push pos

		//literal
		if (s[i++] != 46) goto grp_10_fail;
		//char class
		if (!M5[s[i++]]) goto grp_10_fail;
		while (M5[s[i]]) i++;
		//group 10 end

		goto grp_10_done;

	grp_10_fail:
		i = j;
		break;
	grp_10_done:
	;}
		//group 10 matched

		//group 9 end

		goto grp_9_done;

	grp_9_fail:
		goto next;

	grp_9_done:
		//group 9 matched

		ends[5] = s + i; //store ends

		//group 8 end

		goto grp_8_done;

	grp_8_fail:
		i = j; //restore if not matched

	grp_8_done:
		//group 8 matched

		//group 11 start
		j = i; //push pos

		//literal
		if (s[i++] != 43) goto grp_11_fail;
		//group 12 start
		j = i; //push pos

		starts[6] = s + j; //store starts
		//char class
		if (!M6[s[i++]]) goto grp_12_fail;
		while (M6[s[i]]) i++;
		//group 12 end

		goto grp_12_done;

	grp_12_fail:
		goto next;

	grp_12_done:
		//group 12 matched

		ends[6] = s + i; //store ends

		//group 11 end

		goto grp_11_done;

	grp_11_fail:
		i = j; //restore if not matched

	grp_11_done:
		//group 11 matched


		s += i; //skip starts

		//matched

		ends[0] = s;

		l = starts[0] - prev_s; if (l) { memcpy(d, prev_s, l * sizeof(char)); d += l; }
		if (rlen) {
			for(l = 0; l < rlen; l++) {
				if (repl[l] == '$') {
					l++;
					if (repl[l] == '$') {
						*d++ = '$';
					} else if (repl[l] >= '0' && repl[l] <= '9') {
						i = repl[l] - '0';
						if (repl[l+1] <= '9' && repl[l+1] >= '0') {
							l++;
							i = i * 10 + repl[l] - '0';
						}
						if (i < 6) {
							memcpy(d, starts[i], (ends[i] - starts[i]) * sizeof(char)); d += ends[i] - starts[i];
						}
					}
				} else {*d++ = repl[l];}
			}
		}
		prev_s = ends[0];
	next:
		s++;
	}
	if(prev_s < src + len) { l = src + len - prev_s; memcpy(d, prev_s, l * sizeof(char)); d += l; }
	Py_SIZE(ret) = d - dst;
	return (PyObject*) ret;
}

static PyMethodDef exports[] = {
	{"test", (PyCFunction)test, METH_O, NULL},
	{"findone", (PyCFunction)findone, METH_O, NULL},
	{"findall", (PyCFunction)findall, METH_VARARGS, NULL},
	{"finditer", (PyCFunction)finditer, METH_O, NULL},
	{"sub", (PyCFunction)sub, METH_VARARGS, NULL},
	{NULL, NULL}
};

PyMODINIT_FUNC
initrc_semver(void) {
	str buf = malloc(1536 + sizeof(void*) * 14), b = buf;
	M1 = b, b += 256; // \*
	memset(M1, 0, 256), memset(M1 + 60, 1, 1), memset(M1 + 62, 1, 1);
	M2 = b, b += 256; // \*
	memset(M2, 0, 256), memset(M2 + 60, 1, 3), memset(M2 + 126, 1, 1), memset(M2 + 94, 1, 1);
	M3 = b, b += 256; // \s
	memset(M3, 0, 256), memset(M3 + 9, 1, 2), memset(M3 + 12, 1, 2), memset(M3 + 32, 1, 1);
	M4 = b, b += 256; // \d
	memset(M4, 0, 256), memset(M4 + 48, 1, 10);
	M5 = b, b += 256; // \w
	memset(M5, 0, 256), memset(M5 + 48, 1, 10), memset(M5 + 65, 1, 26), memset(M5 + 95, 1, 1), memset(M5 + 97, 1, 26);
	M6 = b, b += 256; // \*
	memset(M6, 0, 256), memset(M6 + 48, 1, 10), memset(M6 + 65, 1, 26), memset(M6 + 95, 1, 1), memset(M6 + 97, 1, 26), memset(M6 + 46, 1, 1);
	starts = (str*) b, b = b + sizeof(void*) * 7;
	ends = (str*) b;

	PyObject *m = Py_InitModule("rc_semver", exports);
	if (m == NULL) return;

	PyModule_AddObject(m, "source", PyString_FromString("(?x)\n\
#>= v1.2.3-alpha.10.beta.0+build.unicorn.rainbow\n\
([<>]=|[<->~^])?        #< <= > >= ~ ^ =\n\
\\s*\n\
v?(?:(?I:\\d+)|\\*)      #1\n\
(?:\\.(?I:\\d+)|\\.x)?    #2\n\
({4})?                 #3\n\
(?:-(\\w+(?:\\.\\w+)*))?  #alpha.10.beta.0\n\
(?:\\+([\\w.]+))?        #build.unicorn.rainbow\n\
"));
}
