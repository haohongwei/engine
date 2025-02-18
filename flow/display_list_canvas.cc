// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/flow/display_list_canvas.h"

#include "flutter/flow/layers/physical_shape_layer.h"

#include "third_party/skia/include/core/SkMaskFilter.h"
#include "third_party/skia/include/core/SkTextBlob.h"

namespace flutter {

void DisplayListCanvasDispatcher::save() {
  canvas_->save();
}
void DisplayListCanvasDispatcher::restore() {
  canvas_->restore();
}
void DisplayListCanvasDispatcher::saveLayer(const SkRect* bounds,
                                            bool restore_with_paint) {
  TRACE_EVENT0("flutter", "Canvas::saveLayer");
  canvas_->saveLayer(bounds, restore_with_paint ? &paint() : nullptr);
}

void DisplayListCanvasDispatcher::translate(SkScalar tx, SkScalar ty) {
  canvas_->translate(tx, ty);
}
void DisplayListCanvasDispatcher::scale(SkScalar sx, SkScalar sy) {
  canvas_->scale(sx, sy);
}
void DisplayListCanvasDispatcher::rotate(SkScalar degrees) {
  canvas_->rotate(degrees);
}
void DisplayListCanvasDispatcher::skew(SkScalar sx, SkScalar sy) {
  canvas_->skew(sx, sy);
}
// clang-format off
// 2x3 2D affine subset of a 4x4 transform in row major order
void DisplayListCanvasDispatcher::transform2DAffine(
    SkScalar mxx, SkScalar mxy, SkScalar mxt,
    SkScalar myx, SkScalar myy, SkScalar myt) {
  // Internally concat(SkMatrix) gets redirected to concat(SkM44)
  // so we just jump directly to the SkM44 version
  canvas_->concat(SkM44(mxx, mxy, 0, mxt,
                        myx, myy, 0, myt,
                         0,   0,  1,  0,
                         0,   0,  0,  1));
}
// full 4x4 transform in row major order
void DisplayListCanvasDispatcher::transformFullPerspective(
    SkScalar mxx, SkScalar mxy, SkScalar mxz, SkScalar mxt,
    SkScalar myx, SkScalar myy, SkScalar myz, SkScalar myt,
    SkScalar mzx, SkScalar mzy, SkScalar mzz, SkScalar mzt,
    SkScalar mwx, SkScalar mwy, SkScalar mwz, SkScalar mwt) {
  canvas_->concat(SkM44(mxx, mxy, mxz, mxt,
                        myx, myy, myz, myt,
                        mzx, mzy, mzz, mzt,
                        mwx, mwy, mwz, mwt));
}
// clang-format on

void DisplayListCanvasDispatcher::clipRect(const SkRect& rect,
                                           SkClipOp clip_op,
                                           bool is_aa) {
  canvas_->clipRect(rect, clip_op, is_aa);
}
void DisplayListCanvasDispatcher::clipRRect(const SkRRect& rrect,
                                            SkClipOp clip_op,
                                            bool is_aa) {
  canvas_->clipRRect(rrect, clip_op, is_aa);
}
void DisplayListCanvasDispatcher::clipPath(const SkPath& path,
                                           SkClipOp clip_op,
                                           bool is_aa) {
  canvas_->clipPath(path, clip_op, is_aa);
}

void DisplayListCanvasDispatcher::drawPaint() {
  const SkPaint& sk_paint = paint();
  SkImageFilter* filter = sk_paint.getImageFilter();
  if (filter && !filter->asColorFilter(nullptr)) {
    // drawPaint does an implicit saveLayer if an SkImageFilter is
    // present that cannot be replaced by an SkColorFilter.
    TRACE_EVENT0("flutter", "Canvas::saveLayer");
  }
  canvas_->drawPaint(sk_paint);
}
void DisplayListCanvasDispatcher::drawColor(SkColor color, SkBlendMode mode) {
  canvas_->drawColor(color, mode);
}
void DisplayListCanvasDispatcher::drawLine(const SkPoint& p0,
                                           const SkPoint& p1) {
  canvas_->drawLine(p0, p1, paint());
}
void DisplayListCanvasDispatcher::drawRect(const SkRect& rect) {
  canvas_->drawRect(rect, paint());
}
void DisplayListCanvasDispatcher::drawOval(const SkRect& bounds) {
  canvas_->drawOval(bounds, paint());
}
void DisplayListCanvasDispatcher::drawCircle(const SkPoint& center,
                                             SkScalar radius) {
  canvas_->drawCircle(center, radius, paint());
}
void DisplayListCanvasDispatcher::drawRRect(const SkRRect& rrect) {
  canvas_->drawRRect(rrect, paint());
}
void DisplayListCanvasDispatcher::drawDRRect(const SkRRect& outer,
                                             const SkRRect& inner) {
  canvas_->drawDRRect(outer, inner, paint());
}
void DisplayListCanvasDispatcher::drawPath(const SkPath& path) {
  canvas_->drawPath(path, paint());
}
void DisplayListCanvasDispatcher::drawArc(const SkRect& bounds,
                                          SkScalar start,
                                          SkScalar sweep,
                                          bool useCenter) {
  canvas_->drawArc(bounds, start, sweep, useCenter, paint());
}
void DisplayListCanvasDispatcher::drawPoints(SkCanvas::PointMode mode,
                                             uint32_t count,
                                             const SkPoint pts[]) {
  canvas_->drawPoints(mode, count, pts, paint());
}
void DisplayListCanvasDispatcher::drawVertices(const sk_sp<SkVertices> vertices,
                                               SkBlendMode mode) {
  canvas_->drawVertices(vertices, mode, paint());
}
void DisplayListCanvasDispatcher::drawImage(const sk_sp<SkImage> image,
                                            const SkPoint point,
                                            const SkSamplingOptions& sampling,
                                            bool render_with_attributes) {
  canvas_->drawImage(image, point.fX, point.fY, sampling,
                     render_with_attributes ? &paint() : nullptr);
}
void DisplayListCanvasDispatcher::drawImageRect(
    const sk_sp<SkImage> image,
    const SkRect& src,
    const SkRect& dst,
    const SkSamplingOptions& sampling,
    bool render_with_attributes,
    SkCanvas::SrcRectConstraint constraint) {
  canvas_->drawImageRect(image, src, dst, sampling,
                         render_with_attributes ? &paint() : nullptr,
                         constraint);
}
void DisplayListCanvasDispatcher::drawImageNine(const sk_sp<SkImage> image,
                                                const SkIRect& center,
                                                const SkRect& dst,
                                                SkFilterMode filter,
                                                bool render_with_attributes) {
  canvas_->drawImageNine(image.get(), center, dst, filter,
                         render_with_attributes ? &paint() : nullptr);
}
void DisplayListCanvasDispatcher::drawImageLattice(
    const sk_sp<SkImage> image,
    const SkCanvas::Lattice& lattice,
    const SkRect& dst,
    SkFilterMode filter,
    bool render_with_attributes) {
  canvas_->drawImageLattice(image.get(), lattice, dst, filter,
                            render_with_attributes ? &paint() : nullptr);
}
void DisplayListCanvasDispatcher::drawAtlas(const sk_sp<SkImage> atlas,
                                            const SkRSXform xform[],
                                            const SkRect tex[],
                                            const SkColor colors[],
                                            int count,
                                            SkBlendMode mode,
                                            const SkSamplingOptions& sampling,
                                            const SkRect* cullRect,
                                            bool render_with_attributes) {
  canvas_->drawAtlas(atlas.get(), xform, tex, colors, count, mode, sampling,
                     cullRect, render_with_attributes ? &paint() : nullptr);
}
void DisplayListCanvasDispatcher::drawPicture(const sk_sp<SkPicture> picture,
                                              const SkMatrix* matrix,
                                              bool render_with_attributes) {
  if (render_with_attributes) {
    // drawPicture does an implicit saveLayer if an SkPaint is supplied.
    TRACE_EVENT0("flutter", "Canvas::saveLayer");
    canvas_->drawPicture(picture, matrix, &paint());
  } else {
    canvas_->drawPicture(picture, matrix, nullptr);
  }
}
void DisplayListCanvasDispatcher::drawDisplayList(
    const sk_sp<DisplayList> display_list) {
  int save_count = canvas_->save();
  {
    DisplayListCanvasDispatcher dispatcher(canvas_);
    display_list->Dispatch(dispatcher);
  }
  canvas_->restoreToCount(save_count);
}
void DisplayListCanvasDispatcher::drawTextBlob(const sk_sp<SkTextBlob> blob,
                                               SkScalar x,
                                               SkScalar y) {
  canvas_->drawTextBlob(blob, x, y, paint());
}
void DisplayListCanvasDispatcher::drawShadow(const SkPath& path,
                                             const SkColor color,
                                             const SkScalar elevation,
                                             bool transparent_occluder,
                                             SkScalar dpr) {
  flutter::PhysicalShapeLayer::DrawShadow(canvas_, path, color, elevation,
                                          transparent_occluder, dpr);
}

DisplayListCanvasRecorder::DisplayListCanvasRecorder(const SkRect& bounds)
    : SkCanvasVirtualEnforcer(bounds.width(), bounds.height()),
      builder_(sk_make_sp<DisplayListBuilder>(bounds)) {}

sk_sp<DisplayList> DisplayListCanvasRecorder::Build() {
  sk_sp<DisplayList> display_list = builder_->Build();
  builder_.reset();
  return display_list;
}

// clang-format off
void DisplayListCanvasRecorder::didConcat44(const SkM44& m44) {
  // transform4x4 takes a full 4x4 transform in row major order
  builder_->transformFullPerspective(
      m44.rc(0, 0), m44.rc(0, 1), m44.rc(0, 2), m44.rc(0, 3),
      m44.rc(1, 0), m44.rc(1, 1), m44.rc(1, 2), m44.rc(1, 3),
      m44.rc(2, 0), m44.rc(2, 1), m44.rc(2, 2), m44.rc(2, 3),
      m44.rc(3, 0), m44.rc(3, 1), m44.rc(3, 2), m44.rc(3, 3));
}
// clang-format on
void DisplayListCanvasRecorder::didTranslate(SkScalar tx, SkScalar ty) {
  builder_->translate(tx, ty);
}
void DisplayListCanvasRecorder::didScale(SkScalar sx, SkScalar sy) {
  builder_->scale(sx, sy);
}

void DisplayListCanvasRecorder::onClipRect(const SkRect& rect,
                                           SkClipOp clip_op,
                                           ClipEdgeStyle edge_style) {
  builder_->clipRect(rect, clip_op,
                     edge_style == ClipEdgeStyle::kSoft_ClipEdgeStyle);
}
void DisplayListCanvasRecorder::onClipRRect(const SkRRect& rrect,
                                            SkClipOp clip_op,
                                            ClipEdgeStyle edge_style) {
  builder_->clipRRect(rrect, clip_op,
                      edge_style == ClipEdgeStyle::kSoft_ClipEdgeStyle);
}
void DisplayListCanvasRecorder::onClipPath(const SkPath& path,
                                           SkClipOp clip_op,
                                           ClipEdgeStyle edge_style) {
  builder_->clipPath(path, clip_op,
                     edge_style == ClipEdgeStyle::kSoft_ClipEdgeStyle);
}

void DisplayListCanvasRecorder::willSave() {
  builder_->save();
}
SkCanvas::SaveLayerStrategy DisplayListCanvasRecorder::getSaveLayerStrategy(
    const SaveLayerRec& rec) {
  if (rec.fPaint) {
    RecordPaintAttributes(rec.fPaint, DrawType::kSaveLayerOpType);
    builder_->saveLayer(rec.fBounds, true);
  } else {
    builder_->saveLayer(rec.fBounds, false);
  }
  return SaveLayerStrategy::kNoLayer_SaveLayerStrategy;
}
void DisplayListCanvasRecorder::didRestore() {
  builder_->restore();
}

void DisplayListCanvasRecorder::onDrawPaint(const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kFillOpType);
  builder_->drawPaint();
}
void DisplayListCanvasRecorder::onDrawRect(const SkRect& rect,
                                           const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawRect(rect);
}
void DisplayListCanvasRecorder::onDrawRRect(const SkRRect& rrect,
                                            const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawRRect(rrect);
}
void DisplayListCanvasRecorder::onDrawDRRect(const SkRRect& outer,
                                             const SkRRect& inner,
                                             const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawDRRect(outer, inner);
}
void DisplayListCanvasRecorder::onDrawOval(const SkRect& rect,
                                           const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawOval(rect);
}
void DisplayListCanvasRecorder::onDrawArc(const SkRect& rect,
                                          SkScalar startAngle,
                                          SkScalar sweepAngle,
                                          bool useCenter,
                                          const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawArc(rect, startAngle, sweepAngle, useCenter);
}
void DisplayListCanvasRecorder::onDrawPath(const SkPath& path,
                                           const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawPath(path);
}

void DisplayListCanvasRecorder::onDrawPoints(SkCanvas::PointMode mode,
                                             size_t count,
                                             const SkPoint pts[],
                                             const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kStrokeOpType);
  if (mode == SkCanvas::PointMode::kLines_PointMode && count == 2) {
    builder_->drawLine(pts[0], pts[1]);
  } else {
    uint32_t count32 = static_cast<uint32_t>(count);
    // TODO(flar): depending on the mode we could break it down into
    // multiple calls to drawPoints, but how much do we really want
    // to support more than a couple billion points?
    FML_DCHECK(count32 == count);
    builder_->drawPoints(mode, count32, pts);
  }
}
void DisplayListCanvasRecorder::onDrawVerticesObject(const SkVertices* vertices,
                                                     SkBlendMode mode,
                                                     const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawVertices(sk_ref_sp(vertices), mode);
}

void DisplayListCanvasRecorder::onDrawImage2(const SkImage* image,
                                             SkScalar dx,
                                             SkScalar dy,
                                             const SkSamplingOptions& sampling,
                                             const SkPaint* paint) {
  if (paint != nullptr) {
    RecordPaintAttributes(paint, DrawType::kImageOpType);
  }
  builder_->drawImage(sk_ref_sp(image), SkPoint::Make(dx, dy), sampling,
                      paint != nullptr);
}
void DisplayListCanvasRecorder::onDrawImageRect2(
    const SkImage* image,
    const SkRect& src,
    const SkRect& dst,
    const SkSamplingOptions& sampling,
    const SkPaint* paint,
    SrcRectConstraint constraint) {
  if (paint != nullptr) {
    RecordPaintAttributes(paint, DrawType::kImageRectOpType);
  }
  builder_->drawImageRect(sk_ref_sp(image), src, dst, sampling,
                          paint != nullptr, constraint);
}
void DisplayListCanvasRecorder::onDrawImageLattice2(const SkImage* image,
                                                    const Lattice& lattice,
                                                    const SkRect& dst,
                                                    SkFilterMode filter,
                                                    const SkPaint* paint) {
  if (paint != nullptr) {
    // SkCanvas will always construct a paint,
    // though it is a default paint most of the time
    SkPaint default_paint;
    if (*paint == default_paint) {
      paint = nullptr;
    } else {
      RecordPaintAttributes(paint, DrawType::kImageOpType);
    }
  }
  builder_->drawImageLattice(sk_ref_sp(image), lattice, dst, filter,
                             paint != nullptr);
}
void DisplayListCanvasRecorder::onDrawAtlas2(const SkImage* image,
                                             const SkRSXform xform[],
                                             const SkRect src[],
                                             const SkColor colors[],
                                             int count,
                                             SkBlendMode mode,
                                             const SkSamplingOptions& sampling,
                                             const SkRect* cull,
                                             const SkPaint* paint) {
  if (paint != nullptr) {
    RecordPaintAttributes(paint, DrawType::kImageOpType);
  }
  builder_->drawAtlas(sk_ref_sp(image), xform, src, colors, count, mode,
                      sampling, cull, paint != nullptr);
}

void DisplayListCanvasRecorder::onDrawTextBlob(const SkTextBlob* blob,
                                               SkScalar x,
                                               SkScalar y,
                                               const SkPaint& paint) {
  RecordPaintAttributes(&paint, DrawType::kDrawOpType);
  builder_->drawTextBlob(sk_ref_sp(blob), x, y);
}
void DisplayListCanvasRecorder::onDrawShadowRec(const SkPath& path,
                                                const SkDrawShadowRec& rec) {
  // Skia does not expose the SkDrawShadowRec structure in a public
  // header file so we cannot record this operation.
  // See: https://bugs.chromium.org/p/skia/issues/detail?id=12125
  FML_DCHECK(false);
}

void DisplayListCanvasRecorder::onDrawPicture(const SkPicture* picture,
                                              const SkMatrix* matrix,
                                              const SkPaint* paint) {
  if (paint != nullptr) {
    RecordPaintAttributes(paint, DrawType::kSaveLayerOpType);
  }
  builder_->drawPicture(sk_ref_sp(picture), matrix, paint != nullptr);
}

void DisplayListCanvasRecorder::RecordPaintAttributes(const SkPaint* paint,
                                                      DrawType type) {
  int dataNeeded;
  switch (type) {
    case DrawType::kDrawOpType:
      dataNeeded = kDrawMask_;
      break;
    case DrawType::kFillOpType:
      dataNeeded = kPaintMask_;
      break;
    case DrawType::kStrokeOpType:
      dataNeeded = kStrokeMask_;
      break;
    case DrawType::kImageOpType:
      dataNeeded = kImageMask_;
      break;
    case DrawType::kImageRectOpType:
      dataNeeded = kImageRectMask_;
      break;
    case DrawType::kSaveLayerOpType:
      dataNeeded = kSaveLayerMask_;
      break;
    default:
      FML_DCHECK(false);
      return;
  }
  if (paint == nullptr) {
    paint = new SkPaint();
  }
  if ((dataNeeded & kAaNeeded_) != 0 && current_aa_ != paint->isAntiAlias()) {
    builder_->setAntiAlias(current_aa_ = paint->isAntiAlias());
  }
  if ((dataNeeded & kDitherNeeded_) != 0 &&
      current_dither_ != paint->isDither()) {
    builder_->setDither(current_dither_ = paint->isDither());
  }
  if ((dataNeeded & kColorNeeded_) != 0 &&
      current_color_ != paint->getColor()) {
    builder_->setColor(current_color_ = paint->getColor());
  }
  if ((dataNeeded & kBlendNeeded_)) {
    skstd::optional<SkBlendMode> mode_optional = paint->asBlendMode();
    if (mode_optional) {
      SkBlendMode mode = mode_optional.value();
      if (current_blender_ || current_blend_ != mode) {
        builder_->setBlendMode(current_blend_ = mode);
        current_blender_ = nullptr;
      }
    } else {
      if (current_blender_.get() != paint->getBlender()) {
        builder_->setBlender(current_blender_ = sk_ref_sp(paint->getBlender()));
      }
    }
  }
  // invert colors is a Flutter::Paint thing, not an SkPaint thing
  // if ((dataNeeded & invertColorsNeeded_) != 0 &&
  //     currentInvertColors_ != paint->???) {
  //   currentInvertColors_ = paint->invertColors;
  //   addOp_(currentInvertColors_
  //          ? _CanvasOp.setInvertColors
  //          : _CanvasOp.clearInvertColors, 0);
  // }
  if ((dataNeeded & kPaintStyleNeeded_) != 0) {
    if (current_style_ != paint->getStyle()) {
      builder_->setStyle(current_style_ = paint->getStyle());
    }
    if (current_style_ == SkPaint::Style::kStroke_Style) {
      dataNeeded |= kStrokeStyleNeeded_;
    }
  }
  if ((dataNeeded & kStrokeStyleNeeded_) != 0) {
    if (current_stroke_width_ != paint->getStrokeWidth()) {
      builder_->setStrokeWidth(current_stroke_width_ = paint->getStrokeWidth());
    }
    if (current_cap_ != paint->getStrokeCap()) {
      builder_->setStrokeCap(current_cap_ = paint->getStrokeCap());
    }
    if (current_join_ != paint->getStrokeJoin()) {
      builder_->setStrokeJoin(current_join_ = paint->getStrokeJoin());
    }
    if (current_miter_limit_ != paint->getStrokeMiter()) {
      builder_->setStrokeMiter(current_miter_limit_ = paint->getStrokeMiter());
    }
  }
  if ((dataNeeded & kShaderNeeded_) != 0 &&
      current_shader_.get() != paint->getShader()) {
    builder_->setShader(current_shader_ = sk_ref_sp(paint->getShader()));
  }
  if ((dataNeeded & kColorFilterNeeded_) != 0 &&
      current_color_filter_.get() != paint->getColorFilter()) {
    builder_->setColorFilter(current_color_filter_ =
                                 sk_ref_sp(paint->getColorFilter()));
  }
  if ((dataNeeded & kImageFilterNeeded_) != 0 &&
      current_image_filter_.get() != paint->getImageFilter()) {
    builder_->setImageFilter(current_image_filter_ =
                                 sk_ref_sp(paint->getImageFilter()));
  }
  if ((dataNeeded & kPathEffectNeeded_) != 0 &&
      current_path_effect_.get() != paint->getPathEffect()) {
    builder_->setPathEffect(current_path_effect_ =
                                sk_ref_sp(paint->getPathEffect()));
  }
  if ((dataNeeded & kMaskFilterNeeded_) != 0 &&
      current_mask_filter_.get() != paint->getMaskFilter()) {
    builder_->setMaskFilter(current_mask_filter_ =
                                sk_ref_sp(paint->getMaskFilter()));
  }
}

}  // namespace flutter
