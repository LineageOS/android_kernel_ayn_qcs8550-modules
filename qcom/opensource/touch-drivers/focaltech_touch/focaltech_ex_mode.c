/*
 *
 * FocalTech ftxxxx TouchScreen driver.
 *
 * Copyright (c) 2012-2019, Focaltech Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*****************************************************************************
*
* File Name: focaltech_ex_mode.c
*
* Author: Focaltech Driver Team
*
* Created: 2016-08-31
*
* Abstract:
*
* Reference:
*
*****************************************************************************/

/*****************************************************************************
* 1.Included header files
*****************************************************************************/
#include "focaltech_core.h"

/*****************************************************************************
* 2.Private constant and macro definitions using #define
*****************************************************************************/

/*****************************************************************************
* 3.Private enumerations, structures and unions using typedef
*****************************************************************************/
enum _ex_mode {
	MODE_GLOVE = 0,
	MODE_COVER,
	MODE_CHARGER,
	REPORT_RATE,
};

/*****************************************************************************
* 4.Static variables
*****************************************************************************/

/*****************************************************************************
* 5.Global variable or extern global variabls/functions
*****************************************************************************/

/*****************************************************************************
* 6.Static function prototypes
*******************************************************************************/
static int fts_ex_mode_switch(struct fts_fts_data *fts_data, enum _ex_mode mode, u8 value)
{
	int ret = 0;

	switch (mode) {
	case MODE_GLOVE:
		ret = fts_write_reg(fts_data, FTS_REG_GLOVE_MODE_EN, value > 0 ? 1 : 0);
		if (ret < 0)
			FTS_ERROR("MODE_GLOVE switch to %d fail", value);
		break;

	case MODE_COVER:
		ret = fts_write_reg(fts_data, FTS_REG_COVER_MODE_EN, value > 0 ? 1 : 0);
		if (ret < 0)
			FTS_ERROR("MODE_COVER switch to %d fail", value);
		break;

	case MODE_CHARGER:
		ret = fts_write_reg(fts_data, FTS_REG_CHARGER_MODE_EN, value > 0 ? 1 : 0);
		if (ret < 0)
			FTS_ERROR("MODE_CHARGER switch to %d fail", value);
		break;

	case REPORT_RATE:
		ret = fts_write_reg(fts_data, FTS_REG_REPORT_RATE, value);
		if (ret < 0)
			FTS_ERROR("REPORT_RATE switch to %d fail", value);
		break;

	default:
		FTS_ERROR("mode(%d) unsupport", mode);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static ssize_t fts_glove_mode_show(
	struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	struct input_dev *input_dev = fts_data->input_dev;
	int count = 0;
	u8 val = 0;

	mutex_lock(&input_dev->mutex);
	fts_read_reg(fts_data, FTS_REG_GLOVE_MODE_EN, &val);
	count = scnprintf(buf + count, PAGE_SIZE, "Glove Mode:%s\n",
			fts_data->glove_mode ? "On" : "Off");
	count += scnprintf(buf + count, PAGE_SIZE - count,
			"Glove Reg(0xC0):%d\n", val);
	mutex_unlock(&input_dev->mutex);

	return count;
}

static ssize_t fts_glove_mode_store(
	struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	int ret = 0;

	if (FTS_SYSFS_ECHO_ON(buf)) {
		if (!fts_data->glove_mode) {
			FTS_DEBUG("enter glove mode");
			ret = fts_ex_mode_switch(fts_data, MODE_GLOVE, ENABLE);
			if (ret >= 0) {
				fts_data->glove_mode = ENABLE;
			}
		}
	} else if (FTS_SYSFS_ECHO_OFF(buf)) {
		if (fts_data->glove_mode) {
			FTS_DEBUG("exit glove mode");
			ret = fts_ex_mode_switch(fts_data, MODE_GLOVE, DISABLE);
			if (ret >= 0) {
				fts_data->glove_mode = DISABLE;
			}
		}
	}

	FTS_DEBUG("glove mode:%d", fts_data->glove_mode);
	return count;
}

static ssize_t fts_cover_mode_show(
	struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	struct input_dev *input_dev = fts_data->input_dev;
	int count = 0;
	u8 val = 0;

	mutex_lock(&input_dev->mutex);
	fts_read_reg(fts_data, FTS_REG_COVER_MODE_EN, &val);
	count = scnprintf(buf + count, PAGE_SIZE, "Cover Mode:%s\n",
			fts_data->cover_mode ? "On" : "Off");
	count += scnprintf(buf + count, PAGE_SIZE - count,
			"Cover Reg(0xC1):%d\n", val);
	mutex_unlock(&input_dev->mutex);

	return count;
}

static ssize_t fts_cover_mode_store(
	struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	int ret = 0;

	if (FTS_SYSFS_ECHO_ON(buf)) {
		if (!fts_data->cover_mode) {
			FTS_DEBUG("enter cover mode");
			ret = fts_ex_mode_switch(fts_data, MODE_COVER, ENABLE);
			if (ret >= 0) {
				fts_data->cover_mode = ENABLE;
			}
		}
	} else if (FTS_SYSFS_ECHO_OFF(buf)) {
		if (fts_data->cover_mode) {
			FTS_DEBUG("exit cover mode");
			ret = fts_ex_mode_switch(fts_data, MODE_COVER, DISABLE);
			if (ret >= 0) {
				fts_data->cover_mode = DISABLE;
			}
		}
	}

	FTS_DEBUG("cover mode:%d", fts_data->cover_mode);
	return count;
}

static ssize_t fts_charger_mode_show(
	struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	struct input_dev *input_dev = fts_data->input_dev;
	int count = 0;
	u8 val = 0;

	mutex_lock(&input_dev->mutex);
	fts_read_reg(fts_data, FTS_REG_CHARGER_MODE_EN, &val);
	count = scnprintf(buf + count, PAGE_SIZE, "Charger Mode:%s\n",
			fts_data->charger_mode ? "On" : "Off");
	count += scnprintf(buf + count, PAGE_SIZE - count,
			"Charger Reg(0x8B):%d\n", val);
	mutex_unlock(&input_dev->mutex);

	return count;
}

static ssize_t fts_charger_mode_store(
	struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	int ret = 0;

	if (FTS_SYSFS_ECHO_ON(buf)) {
		if (!fts_data->charger_mode) {
			FTS_DEBUG("enter charger mode");
			ret = fts_ex_mode_switch(fts_data, MODE_CHARGER, ENABLE);
			if (ret >= 0) {
				fts_data->charger_mode = ENABLE;
			}
		}
	} else if (FTS_SYSFS_ECHO_OFF(buf)) {
		if (fts_data->charger_mode) {
			FTS_DEBUG("exit charger mode");
			ret = fts_ex_mode_switch(fts_data, MODE_CHARGER, DISABLE);
			if (ret >= 0) {
				fts_data->charger_mode = DISABLE;
			}
		}
	}

	FTS_DEBUG("charger mode:%d", fts_data->glove_mode);
	return count;
}

static ssize_t fts_report_rate_show(
	struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	struct input_dev *input_dev = fts_data->input_dev;
	int count = 0;
	u8 val = 0;

	mutex_lock(&input_dev->mutex);
	fts_read_reg(fts_data, FTS_REG_REPORT_RATE, &val);
	count = scnprintf(buf + count, PAGE_SIZE,
			"Report Rate:%d\n", fts_data->report_rate);
	count += scnprintf(buf + count, PAGE_SIZE - count,
			"Report Rate Reg(0x88):%d\n", val);
	mutex_unlock(&input_dev->mutex);

	return count;
}

static ssize_t fts_report_rate_store(
	struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct fts_fts_data *fts_data = dev_get_drvdata(dev);
	int ret = 0;
	int rate;

	ret = kstrtoint(buf, 16, &rate);
	if (ret)
		return ret;

	if (rate != fts_data->report_rate) {
		ret = fts_ex_mode_switch(fts_data, REPORT_RATE, (u8)rate);
		if (ret >= 0)
			fts_data->report_rate = rate;
	}

	FTS_DEBUG("report rate:%d", fts_data->report_rate);
	return count;
}

/* read and write charger mode
 * read example: cat fts_glove_mode        ---read  glove mode
 * write example:echo 1 > fts_glove_mode   ---write glove mode to 01
 */
static DEVICE_ATTR(fts_glove_mode, S_IRUGO | S_IWUSR,
			fts_glove_mode_show, fts_glove_mode_store);

static DEVICE_ATTR(fts_cover_mode, S_IRUGO | S_IWUSR,
			fts_cover_mode_show, fts_cover_mode_store);

static DEVICE_ATTR(fts_charger_mode, S_IRUGO | S_IWUSR,
			fts_charger_mode_show, fts_charger_mode_store);

static DEVICE_ATTR_RW(fts_report_rate);

static struct attribute *fts_touch_mode_attrs[] = {
	&dev_attr_fts_glove_mode.attr,
	&dev_attr_fts_cover_mode.attr,
	&dev_attr_fts_charger_mode.attr,
	&dev_attr_fts_report_rate.attr,
	NULL,
};

static struct attribute_group fts_touch_mode_group = {
	.attrs = fts_touch_mode_attrs,
};

int fts_ex_mode_recovery(struct fts_fts_data *fts_data)
{
	if (fts_data->glove_mode) {
		fts_ex_mode_switch(fts_data, MODE_GLOVE, ENABLE);
	}

	if (fts_data->cover_mode) {
		fts_ex_mode_switch(fts_data, MODE_COVER, ENABLE);
	}

	if (fts_data->charger_mode) {
		fts_ex_mode_switch(fts_data, MODE_CHARGER, ENABLE);
	}

	if (fts_data->report_rate > 0)
		fts_ex_mode_switch(fts_data, REPORT_RATE, fts_data->report_rate);

	return 0;
}

int fts_ex_mode_init(struct fts_fts_data *fts_data)
{
	int ret = 0;

	fts_data->glove_mode = DISABLE;
	fts_data->cover_mode = DISABLE;
	fts_data->charger_mode = DISABLE;
	fts_data->report_rate = 0;

	ret = sysfs_create_group(&fts_data->dev->kobj, &fts_touch_mode_group);
	if (ret < 0) {
		FTS_ERROR("create sysfs(ex_mode) fail");
		sysfs_remove_group(&fts_data->dev->kobj, &fts_touch_mode_group);
		return ret;
	} else {
		FTS_DEBUG("create sysfs(ex_mode) succeedfully");
	}

	return 0;
}

int fts_ex_mode_exit(struct fts_fts_data *fts_data)
{
	sysfs_remove_group(&fts_data->dev->kobj, &fts_touch_mode_group);
	return 0;
}
